#include "thread.h"

pthread_t CreateThread(void* (*function), threadData* data)
{


	pthread_t id;

	if (data == NULL) {

		if ((pthread_create(&id, NULL, function, NULL)) != 0) {

			return -1;

		}
	}

	else {

		if (pthread_create(&id, NULL, function, (void*)data->data) != 0) {

			return -1;

		}

	}

	return id;

}

int AwaitThread(int threadId)
{

	int x = pthread_join(threadId, NULL);

	return x;
}

void TerminateSelf(void* data)
{

	pthread_exit(data);

}
