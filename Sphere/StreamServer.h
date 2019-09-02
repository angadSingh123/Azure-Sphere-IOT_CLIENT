#pragma once
//Networking
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

//General
#include <stdlib.h>
#include <string.h>
#include <applibs/log.h>

//File descriptor setting
#include <fcntl.h>
#include <signal.h>
#include "Handler.h"


//default port to listen on when the server is in configuration mode
#define CONFIG_PORT 15000

//deafult data buffer length for the socket
#define DEFAULT_BUFFERLEN 128

//Maximum default number of penidng connections
#define DEFAULT_PENDINGLEN 1

typedef struct sockaddr ADDRESS;

typedef struct sockaddr_in ADDRESS_IN;

//Returns a file descriptor to a socket, if listening is set to 1 the socket will accept connections.
int CreateNode(ADDRESS* custom, int listening);

//Set the socket's addresses
int StreamServerStartup(ADDRESS_IN*);

//Enable kernel based check for sockets and returns an epoll file descriptor
int EnableIOSignal(int , EventArgument*, int);

//Close the server socket descriptor and shutdown other processes
int CloseServer(int);

//Accept a client synchronously
int AcceptClient(int socket);

//Tries enabling listening on the socket
static int Listen(int socket);

//ERROR DEFINITIONS
#define FAILURE -255;

#define SOCKET_CLOSE_ERROR -5

#define LISTEN_REG_ERROR -7
