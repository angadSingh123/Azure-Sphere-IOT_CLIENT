//This file is adapted from Microsoft Azure Sphere samples github pages. The file is called epoll_timerfd_utilities.h and is included in almost every project.
//Thanks to MS team. 

#pragma once
#include <errno.h>
#include <applibs/log.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/timerfd.h>


typedef void (*EventFunction) (int);

//This is the structure passed as a refernce to the EnableIOSIgnal
typedef struct  {

	//Call this function when event occurs
	EventFunction eventFunction;

	int descriptor;

	int epollFd;

	int mask;


}EventArgument;


//Collects events from the system
int AwaitNotification(int epoll);

//Create a new Epoll instance on the kernel
int CreateNotifier(void);

//Resgister a socket with an epoll instance
int RegisterNotification(int epoll, int event, const uint32_t, EventArgument* data);

//Create a timer and return the tier file descriptor
int CreateTimerNotification(int epoll, int seconds, int millis, EventArgument* args);

//Stop notifying about the specific event on the specified epoll.
int UnregisterNotification(int epoll, int eventfd);

//ERROR DEFINITIONS
#define EPOLL_CREATE_ERROR -4

#define EPOLL_REG_ERROR -6;