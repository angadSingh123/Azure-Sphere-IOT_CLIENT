#include "Handler.h"

int AwaitNotification(int epoll)
{
	struct epoll_event NewEvent;

	int numberEvents = epoll_wait(epoll, &NewEvent, 1, -1);		

	if (numberEvents == 1 && NewEvent.data.ptr != NULL) {		

		EventArgument* data = NewEvent.data.ptr;

		data->eventFunction(1);
	
	}

	return 0;

	int newEpoll = CreateNotifier();

	if (newEpoll == EPOLL_CREATE_ERROR) return -1;

}

int CreateNotifier(void) {

	int epollFd = EPOLL_CREATE_ERROR;

	epollFd = epoll_create1(0);

	if (epollFd == -1) Log_Debug("Error creating Epoll\n");

	return epollFd;

}

int CreateTimerNotification(int epoll, int seconds, int millis, EventArgument* args) {

	if (seconds == 0 && millis == 0) {
	
		Log_Debug("Time values must be greater than 0.\n");

		return -1;
	}

	struct timespec newTime = { seconds, millis * 1000000 };

	int timerFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

	if (timerFd < 0) {
		
		Log_Debug("Could not creat imer.\n");

		return -1;
	
	}

	struct itimerspec timeSpec = {.it_value = newTime, .it_interval = newTime};

	int iResult = timerfd_settime(timerFd, 0, &timeSpec, NULL);

	if (iResult < 0) {
	
		Log_Debug("Could not set timer.\n");

		return -1;
	
	}

	if (RegisterNotification(epoll, timerFd, EPOLLIN, args) != 0) {
	
		Log_Debug("Failed to register timer.\n");

		return -1;
	
	}

	args->descriptor = timerFd;

	args->epollFd = epoll;

	args->mask = EPOLLIN;

	return timerFd;

}

int UnregisterNotification(int epoll, int eventfd)
{

	int res = 0;

	if ((res = (epoll_ctl(epoll, EPOLL_CTL_DEL, eventfd, NULL))) == -1) {

		Log_Debug("Could not unregister notification. %s\n", strerror(errno));

		return -1;

	}

	return 0;
}

int RegisterNotification(int epoll, int eventfd, const uint32_t mask, EventArgument* Data) {

	Data->epollFd = epoll;

	struct epoll_event EpollEvent = { .data.ptr = Data,.events = mask };

	int iResult = 0;

	if ((iResult = epoll_ctl(epoll, EPOLL_CTL_ADD, eventfd, &EpollEvent)) == -1) {

		Log_Debug("EPOLL ERROR REG 1: %s %d.\n", strerror(errno), errno);

		if ((iResult = epoll_ctl(epoll, EPOLL_CTL_MOD, eventfd, &EpollEvent)) == -1) {

			Log_Debug("EPOLL ERROR REG 2: %s %d.\n", strerror(errno), errno);

			return EPOLL_REG_ERROR;

		}

	}

	return 0;

}
