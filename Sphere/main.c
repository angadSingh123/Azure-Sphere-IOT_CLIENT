#pragma once	
#include <signal.h>
#include "StreamServer.h"
#include "message.h"
#include <sys/types.h>
#include<netdb.h>
#include <errno.h>
#include <soc/mt3620_gpios.h>

volatile sig_atomic_t termination = 0;

volatile sig_atomic_t numberClients = 0;

//defns
void keepAliveHandler(int);

void ListenHandler(int);

void LocalReadHandler(int);

void IotReadHandler(int);

void timerListener(int);

int NetStartup(void);

int NetConnect(void);

int OpenInputs(void);

void TerminationHandler(void);

//file descriptors
int localServer = -1, localClient = -1, epoll = -1, iotClient = -1, timer, keepalive = -1;

//buttons 
int buttonFd, closeButtonFd;

//receive buffer
char buffer[DEFAULT_BUFFERLEN];

//event hadnlers
EventArgument clientReadEvent = { .eventFunction = LocalReadHandler,.mask = EPOLLIN | EPOLLRDHUP };

EventArgument iotClientReadEvent = { .eventFunction = IotReadHandler,.mask = EPOLLIN | EPOLLRDHUP };

//iot address
ADDRESS IOT_SERVER_ADDRESS ;

//local client address
ADDRESS addr_in_localClient;

//address informations
struct addrinfo *addrinfo_iot_server = NULL;

struct addrinfo Hints = { 0 };

//button valuues
static GPIO_Value_Type buttonState = GPIO_Value_High, closeButtonState = GPIO_Value_High;

//util
int times = 0, lastStartup = 0;

Message msg;

Message msgToIotServer = { 0 };

char ABORT[] = "CLOSE|NULL|NULL|";

long m_key = -1;

int main(int argc, char ** argv) {
		
	lastStartup = time(NULL);

	if (OpenInputs() != 0) return -1;

	if ((epoll = CreateNotifier()) < 0) return -1;

	EventArgument timerData = { .eventFunction = timerListener };

	EventArgument keepAliveData = { .eventFunction = keepAliveHandler };

	if ((timer = CreateTimerNotification(epoll, 0, 1, &timerData)) < 0) return -1;	

	if (InitMessage(&msg) != 0) {
	
		Log_Debug("Could not initialise message.\n");
	
	}

	if ((localServer = CreateNode(NULL, 1)) < 0) {
		
		Log_Debug("Could not create a local server.\n");

		termination = 1;

		return -1;
	
	}

	if ((keepalive = CreateTimerNotification(epoll, 900, 0, &keepAliveData)) < 0) return -1;

	EventArgument newData = {.eventFunction = ListenHandler, .mask = EPOLLIN | EPOLLRDHUP};	

	int iResult = EnableIOSignal(localServer, &newData, epoll);
	
	if (iResult < 0) return-1;

	iResult = NetStartup();	

	if (iResult < 0) {
	
		int x = 0;

		do {
			
			iResult = NetStartup();
			sleep(1);
			++x;
		
		} while (iResult < 0 || x < 3 );
	
	}

	iResult = NetConnect();	

	Log_Debug("Connected to iot server.\nLocal server started on port 15000.\n");	

	while (!termination) {
		
		AwaitNotification(epoll);

	}

	send(iotClient, ABORT, strlen(ABORT), 0);

	iResult = CloseServer(iotClient);

	iResult = CloseServer(localServer);

	if (iResult == SOCKET_CLOSE_ERROR) {

		Log_Debug("Could not close server.\n");
	}	

	iResult = close(timer);

	iResult = close(epoll);	

	return 0;
}

void keepAliveHandler(int args)
{	

	char buff[25];

	read(keepalive, buff, 25);	

	lastStartup = (time(NULL) - lastStartup)/1000;

	sprintf(buff, "POST|SERVER|%d|", lastStartup);

	send(iotClient, buff, 25, 0);

}

void ListenHandler(int descriptor) {

	if (numberClients == 0) {  
	
		socklen_t len = sizeof(addr_in_localClient);

		localClient = accept4(localServer, &addr_in_localClient, &len,  SOCK_NONBLOCK | SOCK_CLOEXEC); 		

		int iEpoll = EnableIOSignal(localClient, &clientReadEvent, epoll);

		if (iEpoll != 0) { 					

			return; 
		
		}

		numberClients = 1;		

		struct sockaddr_in* remote = (struct sockaddr_in * ) &addr_in_localClient;		

		Log_Debug("Connected new client: %s:%d\n", inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));

		int rec = sprintf(buffer, "%d", m_key);

		send(localClient, buffer, rec, 0);

	}
	
	else {
	
		int temp = accept(localServer, NULL, NULL);

		close(temp);
	
	}
	
}

void IotReadHandler(int args) {

	memset(buffer, 0, 50);

	int received = recv(iotClient, buffer, DEFAULT_BUFFERLEN, 0);	

	if (received > 0) { Log_Debug("Received %s\n", buffer); 
			
	
	}

	else if (received == 0) {

		struct sockaddr_in* remote = (struct sockaddr_in*) & IOT_SERVER_ADDRESS;

		Log_Debug("Server at %s:%d closed.\n", inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));

		close(iotClient);		

	}


}

void timerListener(int event)
{
	uint64_t timerData;	

	if (read(timer, &timerData, sizeof(uint64_t)) == -1) {
		
		Log_Debug("Could not consume timer.\n");
	
	}


	GPIO_Value_Type newvalue;

	int result = GPIO_GetValue(buttonFd, &newvalue);

	if (result != 0) { Log_Debug("Could not read A button\n"); termination = 1; return; }

	if (newvalue != buttonState) {

		if (newvalue == GPIO_Value_Low) {

			memset(msgToIotServer.OPTIONS, 0, 17);	

			if (!(times % 2))strcpy(msgToIotServer.OPTIONS, "ON");

			else strcpy(msgToIotServer.OPTIONS, "OFF");

			++times;

			int rec = GetBytes(&msgToIotServer, buffer, 100);

			send(iotClient, buffer, rec, 0);
		}

		buttonState = newvalue;
	}

	result = GPIO_GetValue(closeButtonFd, &newvalue);

	if (result != 0) { Log_Debug("Could not read B button\n"); termination = 1; return; }

	if (newvalue != closeButtonState) {

		Log_Debug("Close button pressed\n");

		termination = 1;

		if (UnregisterNotification(epoll, timer) != 0) {

			Log_Debug("error unregistering.\n");
		}

	}

}

void LocalReadHandler(int args) {

	memset(buffer, 0, 128);

	memset(&msg, 0, sizeof(Message));

	msg.DELIMMITER = 124;

	int received = recv(localClient, buffer, DEFAULT_BUFFERLEN, 0);
	
	if (received > 0) {
		
		Log_Debug("Received %s\n", buffer); 
	
		char delim = 124;

		int iResult = ParseMessage(&msg, buffer, received, &delim);
	
		int cmd = strncmp(msg.COMMAND, SET, 4);

		if (cmd == 0) {

			cmd = strncmp(msg.ARGUMENTS, ON, 3);

			if (cmd == 0) {
		
				memset(&msgToIotServer, 0, sizeof(Message));

				if (iResult == 3) {

					Log_Debug("Received normal message.\n");

				}
				
				else if (iResult == 4) {

					Log_Debug("Reveived large message.\n");

					delim = ':';

					msgToIotServer.DELIMMITER = delim;

					iResult = ParseMessage(&msgToIotServer, msg.VAROPTIONS, strlen(msg.VAROPTIONS) + 1, &delim);

					msgToIotServer.DELIMMITER = '|';

					free(msg.VAROPTIONS);

				}

				else {

					struct sockaddr_in* remote = (struct sockaddr_in*) & addr_in_localClient;

					Log_Debug("Could not parse message from %s.\n", inet_ntoa(remote->sin_addr));

				}
			
			
			}
		

		}
		
		send(localClient, "POST|SELF|ACKNOWLEDGED|", 23, 0);

	}
	
	else if (received == 0) {
	
		struct sockaddr_in* remote = (struct sockaddr_in*) & addr_in_localClient;

		Log_Debug("Client at %s:%d closed.\n", inet_ntoa(remote->sin_addr), ntohs(remote->sin_port));

		close(localClient);

		numberClients = 0;
		
	}

}

void TerminationHandler(void)
{
	termination = 1;
	Log_Debug("Terminated\n");

}

int NetStartup() {

	memset(&Hints, 0, sizeof(Hints));

	Hints.ai_family = AF_UNSPEC;

	Hints.ai_socktype = SOCK_STREAM;

	Hints.ai_protocol = IPPROTO_TCP;

	int result =  getaddrinfo("harsimiot.eastus.cloudapp.azure.com", "8080", &Hints, &addrinfo_iot_server);

	Log_Debug("%s\n",gai_strerror(result));

	return result;

}

int NetConnect() {
	
	struct addrinfo* ptr = NULL;

	for (ptr = addrinfo_iot_server; ptr != NULL; ptr = ptr->ai_next) {
	
		Log_Debug("Found server: %s\n", ptr->ai_canonname);			
		
		if ((iotClient = socket(ptr->ai_family, SOCK_STREAM  , ptr->ai_protocol)) < 0) {
		
			Log_Debug("Problem creating pipe. %d\n", iotClient);

			break;

		}
		
		int iResult;

		if ( (iResult = connect(iotClient, ptr->ai_addr, ptr->ai_addrlen) ) < 0 && errno == 115) {
		
			Log_Debug("Connecting to server. %d  %s\n", errno, strerror(errno));			
		
		}
		else if (iResult == 0){
			
			Log_Debug("Connecting to server.\n");
		
		}
		else{
		
			Log_Debug("Problem connecting to server.\n");
			
		}

		if ((iResult = EnableIOSignal(iotClient, &iotClientReadEvent, epoll)) != 0){
		
			Log_Debug("Could not set NB mode.\n");
			goto fail;
		}

		IOT_SERVER_ADDRESS = *(ptr->ai_addr);

		break;

	}

	freeaddrinfo(addrinfo_iot_server);

	send(iotClient, "POST|SERVER|KEY|", 16, 0);

	send(iotClient, "ECHOBACK|SERVER|KEY|", 20, 0);

	memset(buffer, 0, 128);

	memchr(&msg, 0, sizeof(msg));

	sleep(2);

	int rec = recv(iotClient, buffer, 128, 0);

	char delim = '|';

	rec = ParseMessage(&msg, buffer, rec, &delim);

	if (rec == 3) {	
		m_key = atol(msg.OPTIONS);	
	}

	else if (rec == 4) {		
		m_key = atol(msg.VAROPTIONS);
	}	

	return 0;

fail:
	
	freeaddrinfo(addrinfo_iot_server);

	return -1;	

}

int OpenInputs()
{
	//Handler what happens when the process is killed;
	struct sigaction killAction;
	memset(&killAction, 0, sizeof(sigaction));
	killAction.sa_handler = TerminationHandler;
	sigaction(SIGTERM, &killAction, NULL);	

	if ((buttonFd = GPIO_OpenAsInput(12)) < 0) {

		Log_Debug("Could not open button A. %s %d \n", strerror(errno), errno);

		return -1;

	}

	if ((closeButtonFd = GPIO_OpenAsInput(13)) < 0) {

		Log_Debug("Could not open button B. %s %d \n", strerror(errno), errno);

		return -1;

	}

	return 0;
}

