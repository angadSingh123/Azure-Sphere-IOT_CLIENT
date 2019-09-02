#include "StreamServer.h"

int CreateNode(ADDRESS* myAddr, int listener){

	 int sock = -1, retFd = -1, result;

	 struct sockaddr_in addr;

	 if (myAddr == NULL) myAddr = &addr;		 

	 if ((sock = socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_TCP)) < 0) {
		 
		 Log_Debug("Error initialising socket\n");

		 return -1;
	 }

	 StreamServerStartup(myAddr);

	 result = bind(sock, (struct sockaddr*) myAddr, sizeof(*myAddr));
		 
	if (result != 0) {
	 
		 Log_Debug("Error binding socket\n");

		 return -2;
	 
	 }

	 int enableReuse = 1;

	 int rebind = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse));

	 if (rebind != 0) {
	 
		 Log_Debug("Error setting socket option\n");

		 return -3;
	 
	 }

	 int listen = (listener == 1) ? Listen(sock) : 0;

	 if (listen != 0) {
		
		 goto failure;
	 
	 }

	 retFd = sock;	 

	 return retFd;

 failure:

	 CloseServer(sock);

	 return FAILURE;

}

int StreamServerStartup(ADDRESS_IN*myAddr)
{
	memset(myAddr, 0, sizeof(*myAddr));

	myAddr->sin_family = AF_INET;
	myAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr->sin_port = htons(CONFIG_PORT);

	return 0;
}

int EnableIOSignal(int sockFd, EventArgument *eventArg, int newEpoll)
{

	eventArg->descriptor = sockFd;			

	int epollResult = RegisterNotification(newEpoll, sockFd, eventArg->mask, eventArg);

	if (epollResult != 0) { Log_Debug("Error registering epoll.\n"); return -1; }

	return 0;
	
}




 int AcceptClient(int socket)
 {
	 struct sockaddr in_addr;

	 socklen_t socklen = sizeof(in_addr);

	 int client = accept(socket, &in_addr, &socklen);

	 return client;

 }

int CloseServer(int socket) {

	if (socket >= 0)

	{
		int result = close(socket);

		if (result != 0) return SOCKET_CLOSE_ERROR;

	}

	return 0;

}

static int Listen(int socket)
{
	int result = listen(socket, DEFAULT_PENDINGLEN);

	if (result != 0) return LISTEN_REG_ERROR;

	return 0;
}
