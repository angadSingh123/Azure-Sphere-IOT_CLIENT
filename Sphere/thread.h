#pragma once
#include <pthread.h>

typedef struct {

	void* data;

}  threadData;

pthread_t CreateThread(void* (*function), threadData* data);

int AwaitThread(int threadId);

void TerminateSelf(void*);
