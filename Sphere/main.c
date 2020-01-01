#pragma once	
#include <signal.h>

#include "StreamServer.h"
#include "message.h"
#include "thread.h"

#include "ledMatrix.h"

#include <sys/types.h>
#include<netdb.h>
#include <errno.h>
#include <time.h>

#include <soc/mt3620_gpios.h>
#include <applibs/storage.h>

volatile sig_atomic_t termination = 0;

volatile sig_atomic_t numberClients = 0;

//defns
void keepAliveHandler(int);

void ListenHandler(int);

void LocalReadHandler(int);

void IotReadHandler(int);

void timerListener(int);

void flashLights();

int NetStartup(void);

int NetConnect(void);

int OpenInputs(void);

int closeInputs();

static long ReadMutableFile(void);

static long WriteToMutableFile(long);

void TerminationHandler(void);

void* ledFunction(void* data);

static inline void setBuffer(char*, char*, int);

//file descriptors
int localServer = -1, localClient = -1, epoll = -1, iotClient = -1, timer = -1, keepalive = -1;

//buttons 
int buttonFd, closeButtonFd;

//pins for controlling the matrix
int cols[8] = {35, 32, 34, 31, 33, 69, 68, 67};
int dataFd = 39, clockFd = 37, latchFd = 40;

//ON BOARD rgb FOR led1
int r, g, b;

//receive buffer
char buffer[DEFAULT_BUFFERLEN];

//event hadnlers
EventArgument clientReadEvent = { .eventFunction = LocalReadHandler,.mask = EPOLLIN | EPOLLRDHUP };

EventArgument iotClientReadEvent = { .eventFunction = IotReadHandler,.mask = EPOLLIN | EPOLLRDHUP };

//iot address
ADDRESS IOT_SERVER_ADDRESS;

//local client address
ADDRESS addr_in_localClient;

//address informations
struct addrinfo* addrinfo_iot_server = NULL;

struct addrinfo Hints = { 0 };

//matrix delay
const struct timespec DELAY = { 0,  1500000 };

//flash delay
const struct timespec DELAY2 = { 0,  5000000 };

//button valuues
static GPIO_Value_Type buttonState = GPIO_Value_High, closeButtonState = GPIO_Value_High;

//util
int times = 0, lastStartup = 0;

Message msg;

Message msgToIotServer = { 0 };

char ABORT[] = "CLOSE|NULL|NULL|";

//message to be displayed on the matrix
char* DISPLAY_BUFFER = ABORT;

//mutual lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//message length
int bufferLength = 16;

//index of the word 
int idx = 0;

long m_key = -1;

long m_remoteKey = -1;

//Letter data that is shifted out
unsigned char  ALL[8] = { 0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111 };

unsigned char  EX[8] = { 0b00000000,0b00010000,0b00010000,0b00010000,0b00010000,0b00000000,0b00010000,0b00000000 };

unsigned char  A[8] = { 0b00000000,0b00111100,0b01100110,0b01100110,0b01111110,0b01100110,0b01100110,0b01100110 };

unsigned char  B[8] = { 0b01111000,0b01001000,0b01001000,0b01110000,0b01001000,0b01000100,0b01000100,0b01111100 };

unsigned char  C[8] = { 0b00000000,0b00011110,0b00100000,0b01000000,0b01000000,0b01000000,0b00100000,0b00011110 };

unsigned char  D[8] = { 0b00000000,0b00111000,0b00100100,0b00100010,0b00100010,0b00100100,0b00111000,0b00000000 };

unsigned char  E[8] = { 0b00000000,0b00111100,0b00100000,0b00111000,0b00100000,0b00100000,0b00111100,0b00000000 };

unsigned char  F[8] = { 0b00000000,0b00111100,0b00100000,0b00111000,0b00100000,0b00100000,0b00100000,0b00000000 };

unsigned char  G[8] = { 0b00000000,0b00111110,0b00100000,0b00100000,0b00101110,0b00100010,0b00111110,0b00000000 };

unsigned char  H[8] = { 0b00000000,0b00100100,0b00100100,0b00111100,0b00100100,0b00100100,0b00100100,0b00000000 };

unsigned char  I[8] = { 0b00000000,0b00111000,0b00010000,0b00010000,0b00010000,0b00010000,0b00111000,0b00000000 };

unsigned char  J[8] = { 0b00000000,0b00011100,0b00001000,0b00001000,0b00001000,0b00101000,0b00111000,0b00000000 };

unsigned char  K[8] = { 0b00000000,0b00100100,0b00101000,0b00110000,0b00101000,0b00100100,0b00100100,0b00000000 };

unsigned char  L[8] = { 0b00000000,0b00100000,0b00100000,0b00100000,0b00100000,0b00100000,0b00111100,0b00000000 };

unsigned char  M[8] = { 0b00000000,0b00000000,0b01000100,0b10101010,0b10010010,0b10000010,0b10000010,0b00000000 };

unsigned char  N[8] = { 0b00000000,0b00100010,0b00110010,0b00101010,0b00100110,0b00100010,0b00000000,0b00000000 };

unsigned char  O[8] = { 0b00000000,0b00111100,0b01000010,0b01000010,0b01000010,0b01000010,0b00111100,0b00000000 };

unsigned char  P[8] = { 0b00000000,0b00111000,0b00100100,0b00100100,0b00111000,0b00100000,0b00100000,0b00000000 };

unsigned char  Q[8] = { 0b00000000,0b00111100,0b01000010,0b01000010,0b01000010,0b01000110,0b00111110,0b00000001 };

unsigned char  R[8] = { 0b00000000,0b00111000,0b00100100,0b00100100,0b00111000,0b00100100,0b00100100,0b00000000 };

unsigned char  S[8] = { 0b00000000,0b00111100,0b00100000,0b00111100,0b00000100,0b00000100,0b00111100,0b00000000 };

unsigned char  T[8] = { 0b00000000,0b01111100,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00000000 };

unsigned char  U[8] = { 0b00000000,0b01000010,0b01000010,0b01000010,0b01000010,0b00100100,0b00011000,0b00000000 };

unsigned char  V[8] = { 0b00000000,0b00100010,0b00100010,0b00100010,0b00010100,0b00010100,0b00001000,0b00000000 };

unsigned char  W[8] = { 0b00000000,0b10000010,0b10010010,0b01010100,0b01010100,0b00101000,0b00000000,0b00000000 };

unsigned char  X[8] = { 0b00000000,0b01000010,0b00100100,0b00011000,0b00011000,0b00100100,0b01000010,0b00000000 };

unsigned char  Y[8] = { 0b00000000,0b01000100,0b00101000,0b00010000,0b00010000,0b00010000,0b00010000,0b00000000 };

unsigned char  Z[8] = { 0b00000000,0b00111100,0b00000100,0b00001000,0b00010000,0b00100000,0b00111100,0b00000000 };

unsigned char  NONE[8] = { 0,0,0,0,0,0,0,0 };

int main(int argc, char** argv) {

	ConstructMessage(&msgToIotServer, "SEND", "", "ON");

	msgToIotServer.DELIMMITER = DEFAULT_DELIM;

	m_remoteKey = ReadMutableFile();

	sprintf(msgToIotServer.ARGUMENTS, "%d", m_remoteKey);

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

	EventArgument newData = { .eventFunction = ListenHandler,.mask = EPOLLIN | EPOLLRDHUP };

	int iResult = EnableIOSignal(localServer, &newData, epoll);

	//if (iResult < 0) return-1;

	//iResult = NetStartup();	

	//if (iResult < 0) {
	//
	//	int x = 0;

	//	do {
	//		
	//		iResult = NetStartup();
	//		sleep(1);
	//		++x;
	//	
	//	} while (iResult < 0 || x < 3 );
	//
	//}

	//iResult = NetConnect();	

	Log_Debug("Connected to iot server.\nLocal server started on port 15000.\n");

	//block infinitely
	int blocker = -1;

	pthread_t ledThread = CreateThread(ledFunction, NULL);

	while (!termination) {

		AwaitNotification(epoll, &blocker);

	}

	AwaitThread(ledThread);

	send(iotClient, ABORT, strlen(ABORT), 0);

	iResult = CloseServer(iotClient);

	iResult = CloseServer(localServer);

	if (iResult == SOCKET_CLOSE_ERROR) {

		Log_Debug("Could not close server.\n");
	}

	iResult = close(timer);

	iResult = close(epoll);

	closeInputs();

	if (DISPLAY_BUFFER != NULL) free(DISPLAY_BUFFER);

	return 0;
}

void keepAliveHandler(int args)
{

	char buff[25];

	read(keepalive, buff, 25);

	lastStartup = (time(NULL) - lastStartup) / 1000;

	sprintf(buff, "POST|SERVER|%d|", lastStartup);

	send(iotClient, buff, 25, 0);

}

void ListenHandler(int descriptor) {

	if (numberClients == 0) {

		socklen_t len = sizeof(addr_in_localClient);

		localClient = accept4(localServer, &addr_in_localClient, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);

		int iEpoll = EnableIOSignal(localClient, &clientReadEvent, epoll);

		if (iEpoll != 0) {

			return;

		}

		numberClients = 1;

		struct sockaddr_in* remote = (struct sockaddr_in*) & addr_in_localClient;

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

	if (received > 0) {
		Log_Debug("Received %s\n", buffer);


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

			memset(buffer, 0, 100);

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

void flashLights()
{

	GPIO_SetValue(r, 0);

	nanosleep(&DELAY2, NULL);

	GPIO_SetValue(r, 1);

	GPIO_SetValue(g, 0);

	nanosleep(&DELAY2, NULL);

	GPIO_SetValue(g, 1);

	GPIO_SetValue(b, 0);

	nanosleep(&DELAY2, NULL);

	GPIO_SetValue(b, 1);
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

		if (iResult < 0) {

			Log_Debug("Local client's message was not parsed.\n");

			return;

		}

		int cmd = strncmp(msg.COMMAND, SET, 4);

		if (cmd == 0) {

			cmd = strncmp(msg.ARGUMENTS, ON, 3);

			if (cmd == 0) {

				memset(&msgToIotServer, 0, sizeof(Message));

				delim = ':';

				msgToIotServer.DELIMMITER = delim;

				if (iResult == 3) {

					Log_Debug("Received normal message.\n");

					iResult = ParseMessage(&msgToIotServer, msg.OPTIONS, strlen(msg.OPTIONS) + 1, &delim);

				}

				else if (iResult == 4) {

					Log_Debug("Reveived large message.\n");

					iResult = ParseMessage(&msgToIotServer, msg.VAROPTIONS, strlen(msg.VAROPTIONS) + 1, &delim);

					free(msg.VAROPTIONS);

				}

				else {

					struct sockaddr_in* remote = (struct sockaddr_in*) & addr_in_localClient;

					Log_Debug("Could not parse message from %s.\n", inet_ntoa(remote->sin_addr));

				}

				if (iResult < 0) {

					Log_Debug("Error parsing new message. Reverting to default message.\n");

					ConstructMessage(&msgToIotServer, "SEND", "", "ON");

					sprintf(msgToIotServer.ARGUMENTS, "%d", m_remoteKey);

				}

				msgToIotServer.DELIMMITER = '|';

			}

			m_remoteKey = atol(msgToIotServer.ARGUMENTS);

			WriteToMutableFile(m_remoteKey);

			char buffer[50];

			int written = sprintf(buffer, "POST|SELF|%d is the new key.|", m_remoteKey);

			send(localClient, buffer, written, 0);

		}

		cmd = strncmp(msg.COMMAND, POST, 5);

		if (cmd == 0) {

			cmd = strncmp(msg.ARGUMENTS, SELF, 5);

			if (cmd != 0) return;

			if (iResult >= 3) {
				

				if (iResult == 4) {

					pthread_mutex_lock(&lock);

					if (DISPLAY_BUFFER != NULL && DISPLAY_BUFFER != ABORT) {

						free(DISPLAY_BUFFER);

					}

					bufferLength = strlen(msg.VAROPTIONS);

					DISPLAY_BUFFER = (char*)malloc((sizeof(char) * bufferLength) + 1);

					strcpy((char*)DISPLAY_BUFFER, (char*)msg.VAROPTIONS);

					idx = 0;

					pthread_mutex_unlock(&lock);

					free(msg.VAROPTIONS);

				}
				
				else {

					pthread_mutex_lock(&lock);

					if (DISPLAY_BUFFER != NULL && DISPLAY_BUFFER != ABORT) {

						free(DISPLAY_BUFFER);

					}

					bufferLength = strlen(msg.OPTIONS);

					DISPLAY_BUFFER = (char*)malloc((sizeof(char) * bufferLength) + 1);

					strcpy((char*)DISPLAY_BUFFER, (char*)msg.OPTIONS);

					idx = 0;

					pthread_mutex_unlock(&lock);

				}

			}
			

		}

		flashLights();

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

void* ledFunction(void* data)
{
	clock_t prev = 0;

	int count = 0;

	char RENDER[8] = { 0, 0, 0, 0, 0, 0, 0,0 };
	char BUFFER[8] = { 0, 0, 0, 0, 0, 0, 0,0 };

	memcpy(RENDER, ALL, 64);

	while (!termination) {

		if (clock() - prev > 12000) {

			shiftLeft((unsigned char*)RENDER, (unsigned char*)BUFFER);

			prev = clock();

			if (count % 8 == 0) {

				pthread_mutex_lock(&lock);

				setBuffer((char*)DISPLAY_BUFFER, (char*)BUFFER, idx);

				if (idx == bufferLength) idx = 0; else idx++;

				pthread_mutex_unlock(&lock);

			}

			count++;

		}

		for (int j = 0; j < 8; j++) {

			GPIO_SetValue(cols[j], 0);

			write8_m((unsigned char*) & (RENDER[j]), &latchFd, &dataFd, &clockFd);

			nanosleep(&DELAY, NULL);

			write8((unsigned char*) & (NONE), &latchFd, &dataFd, &clockFd);

			GPIO_SetValue(cols[j], 1);

		}

	}

	TerminateSelf(&NONE);
}

int NetStartup() {

	memset(&Hints, 0, sizeof(Hints));

	Hints.ai_family = AF_UNSPEC;

	Hints.ai_socktype = SOCK_STREAM;

	Hints.ai_protocol = IPPROTO_TCP;

	int result = getaddrinfo("harsimiot.eastus.cloudapp.azure.com", "8080", &Hints, &addrinfo_iot_server);

	Log_Debug("%s\n", gai_strerror(result));

	return result;

}

int NetConnect() {

	struct addrinfo* ptr = NULL;

	for (ptr = addrinfo_iot_server; ptr != NULL; ptr = ptr->ai_next) {

		Log_Debug("Found server: %s\n", ptr->ai_canonname);

		if ((iotClient = socket(ptr->ai_family, SOCK_STREAM, ptr->ai_protocol)) < 0) {

			Log_Debug("Problem creating pipe. %d\n", iotClient);

			break;

		}

		int iResult;

		if ((iResult = connect(iotClient, ptr->ai_addr, ptr->ai_addrlen)) < 0 && errno == 115) {

			Log_Debug("Connecting to server. %d  %s\n", errno, strerror(errno));

		}
		else if (iResult == 0) {

			Log_Debug("Connecting to server.\n");

		}
		else {

			Log_Debug("Problem connecting to server.\n");

		}

		if ((iResult = EnableIOSignal(iotClient, &iotClientReadEvent, epoll)) != 0) {

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
		free(msg.VAROPTIONS);
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

	if ((dataFd = GPIO_OpenAsOutput(39, GPIO_OutputMode_PushPull, 0)) < 0) {

		Log_Debug("Could not open data. %s %d \n", strerror(errno), errno);

		return -1;

	}

	if ((clockFd = GPIO_OpenAsOutput(37, GPIO_OutputMode_PushPull, 0)) < 0) {

		Log_Debug("Could not open clk. %s %d \n", strerror(errno), errno);

		return -1;

	}

	if ((r = GPIO_OpenAsOutput(8, GPIO_OutputMode_PushPull, 0)) < 0) {

		Log_Debug("Could not open r. %s %d \n", strerror(errno), errno);

		return -1;

	}

	if ((g = GPIO_OpenAsOutput(9, GPIO_OutputMode_PushPull, 0)) < 0) {

		Log_Debug("Could not open g. %s %d \n", strerror(errno), errno);

		return -1;

	}


	if ((b = GPIO_OpenAsOutput(10, GPIO_OutputMode_PushPull, 0)) < 0) {

		Log_Debug("Could not open b. %s %d \n", strerror(errno), errno);

		return -1;

	}


	if ((latchFd = GPIO_OpenAsOutput(40, GPIO_OutputMode_PushPull, 0)) < 0) {

		Log_Debug("Could not open latch. %s %d \n", strerror(errno), errno);

		return -1;

	}

	for (int i = 0; i < 8; i++) {
	
		Log_Debug("GPIO %d\n", cols[i]);

		if ((cols[i] = GPIO_OpenAsOutput(cols[i], GPIO_OutputMode_OpenDrain, 0)) < 0) {

			Log_Debug("Could not open gpio. %s %d \n", strerror(errno), errno);

			return -1;

		}
	
	}


	return 0;
}

//Following sources from https://github.com/Azure/azure-sphere-samples/blob/master/Samples/MutableStorage/MutableStorage/main.c#L84

static long WriteToMutableFile(long value)

{

	int fd = Storage_OpenMutableFile();

	if (fd < 0) {

		Log_Debug("ERROR: Could not open mutable file:  %s (%d).\n", strerror(errno), errno);

		return;

	}

	ssize_t ret = write(fd, &value, sizeof(value));

	if (ret < 0) {

		// If the file has reached the maximum size specified in the application manifest,

		// then -1 will be returned with errno EDQUOT (122)

		Log_Debug("ERROR: An error occurred while writing to mutable file:  %s (%d).\n",

			strerror(errno), errno);

	}
	else if (ret < sizeof(value)) {

		// For simplicity, this sample logs an error here. In the general case, this should be

		// handled by retrying the write with the remaining data until all the data has been

		// written.

		Log_Debug("ERROR: Only wrote %d of %d bytes requested\n", ret, (int)sizeof(value));

	}

	close(fd);

}

static long ReadMutableFile(void)

{

	int fd = Storage_OpenMutableFile();

	if (fd < 0) {

		Log_Debug("ERROR: Could not open mutable file:  %s (%d).\n", strerror(errno), errno);

		return -1;

	}

	long value = 0;

	ssize_t ret = read(fd, &value, sizeof(value));

	if (ret < 0) {

		Log_Debug("ERROR: An error occurred while reading file:  %s (%d).\n", strerror(errno),

			errno);

	}

	close(fd);



	if (ret < sizeof(value)) {

		return 0;

	}



	return value;

}

static inline void setBuffer(char* string, char* BUFFER, int idx) {

	if (string == NULL || BUFFER == NULL) return;

	switch (string[idx]) {

	case 32: memcpy(BUFFER, NONE, 64); break;
	case 65: memcpy(BUFFER, A, 64); break;
	case 66: memcpy(BUFFER, B, 64); break;
	case 67: memcpy(BUFFER, C, 64); break;
	case 68: memcpy(BUFFER, D, 64); break;
	case 69: memcpy(BUFFER, E, 64); break;
	case 70: memcpy(BUFFER, F, 64); break;
	case 71: memcpy(BUFFER, G, 64); break;
	case 72: memcpy(BUFFER, H, 64); break;
	case 73: memcpy(BUFFER, I, 64); break;
	case 74: memcpy(BUFFER, J, 64); break;
	case 75: memcpy(BUFFER, K, 64); break;
	case 76: memcpy(BUFFER, L, 64); break;
	case 77: memcpy(BUFFER, M, 64); break;
	case 78: memcpy(BUFFER, N, 64); break;
	case 79: memcpy(BUFFER, O, 64); break;
	case 80: memcpy(BUFFER, P, 64); break;
	case 81: memcpy(BUFFER, Q, 64); break;
	case 82: memcpy(BUFFER, R, 64); break;
	case 83: memcpy(BUFFER, S, 64); break;
	case 84: memcpy(BUFFER, T, 64); break;
	case 85: memcpy(BUFFER, U, 64); break;
	case 86: memcpy(BUFFER, V, 64); break;
	case 87: memcpy(BUFFER, W, 64); break;
	case 88: memcpy(BUFFER, X, 64); break;
	case 89: memcpy(BUFFER, Y, 64); break;
	case 90: memcpy(BUFFER, Z, 64); break;
	default: memcpy(BUFFER, ALL, 64); break;

	}

}

int closeInputs()
{
	close(closeButtonFd);
	close(buttonFd);
	close(dataFd);
	close(clockFd);
	close(latchFd);
	for (int i = 0; i < 8; i++) close(cols[i]);

	return 0;
}

