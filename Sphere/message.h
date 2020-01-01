#pragma once
#include<string.h>
#include "Argument.h"
#include "Command.h"
#include "Parametre.h"

#define DEFAULT_PARAM_LENGTH 18
#define DEFAULT_DELIM 124
#define MSG_TOO_LARGE -2

typedef struct {

	char COMMAND[DEFAULT_PARAM_LENGTH];
	char ARGUMENTS[DEFAULT_PARAM_LENGTH];
	char OPTIONS[DEFAULT_PARAM_LENGTH];
	char *VAROPTIONS;
	char DELIMMITER;
	char varied;
	
	//Useful when parsing messages.
	/*char  cmdType;
	char argType;
	char opType;*/	

} Message;

//Copy from source into destination and return the numer of bytes copied.
int CopyAndReturnBytesCopied(char* restrict source, char* restrict destination, char* delim) {

	unsigned char copied = 0;

	while (*(source + copied) != *delim) {

		*(destination + copied) = *(source + copied);

		++copied;

	}

	return copied;

}

int InitMessage(Message *myMsg) {
	
	memset(myMsg, 0, sizeof(*myMsg));
	myMsg->DELIMMITER = DEFAULT_DELIM;
	return 0;
}

int ConstructMessage(Message *msg, const char* Command, const char* Args, const char* Parametres) {	

	strcpy(msg->COMMAND, Command);

	strcpy(msg->ARGUMENTS, Args);

	if (strlen(Parametres) >= DEFAULT_PARAM_LENGTH) {
	
		strcpy(msg->OPTIONS, "MSG2LRG");
		msg->VAROPTIONS = Parametres;
		return MSG_TOO_LARGE;

	}
		
	else strcpy(msg->OPTIONS, Parametres);

	return 0;
}

//Convert the message into an array of bytes that is saved at address pointed by Array.
int GetBytes(Message *restrict msg, char *restrict Array, int maxlen) {
	
	char x = 0;

	int currentIndex = 0;

	//Assume starting from the index number 0
	currentIndex = CopyAndReturnBytesCopied(msg->COMMAND, Array, &x) ; 
	
	memset((Array + currentIndex), msg->DELIMMITER, 1);	

	currentIndex += CopyAndReturnBytesCopied(msg->ARGUMENTS, (Array + (++currentIndex)), &x) ;	

	memset((Array + (currentIndex)), msg->DELIMMITER, 1);	

	currentIndex += CopyAndReturnBytesCopied(msg->OPTIONS, (Array + (++currentIndex)), &x);

	memset((Array + (currentIndex)), msg->DELIMMITER, 1);	

	return ++currentIndex;
}

//Convert byte array into a message structure. Return the number of components parsed.
int ParseMessage(Message* msg, char* byteSource, int lengthstr, char* delim) {

	char returner = 0;

	if (!msg || !byteSource) return -1;

	if (lengthstr <= 0) return -1;

	//store indeces
	int delimLoc[3] = {0,0,0};

	//find the delimiter in the first eight bytes since all commands and arguments have a maximum possible length of 17 bytes
	char* search = memchr(byteSource, *delim, DEFAULT_PARAM_LENGTH);

	if (search == NULL) return -1;

	delimLoc[0] = search - byteSource;	

	//find second delimmiter
	search = memchr(++search, *delim, DEFAULT_PARAM_LENGTH);

	if (search == NULL) return -2;

	delimLoc[1] = search - byteSource;

	//find third delimiiter
	search = memchr(++search, *delim, (lengthstr) - (delimLoc[1]));

	if (search == NULL) return -3;

	delimLoc[2] = search - byteSource;

	//Now copy the message into the message structure

	if (delimLoc[0]) {	
		CopyAndReturnBytesCopied(byteSource, msg->COMMAND, &msg->DELIMMITER);	
		returner = 1;
	}

	if (delimLoc[1] - delimLoc[0] > 1) {	
		CopyAndReturnBytesCopied(byteSource + delimLoc[0] + 1, msg->ARGUMENTS, &msg->DELIMMITER);		
		returner = 2;
	}

	int length = delimLoc[2] - delimLoc[1];

	if (length <= 1) {
	
		return returner;

	}

	else if (length <= DEFAULT_PARAM_LENGTH) {

		CopyAndReturnBytesCopied(byteSource + delimLoc[1] + 1, msg->OPTIONS, &msg->DELIMMITER);

		returner = 3;
	}

	else if (length > DEFAULT_PARAM_LENGTH) {
		
		msg->VAROPTIONS = malloc(sizeof(char) * (length));

		if (msg->VAROPTIONS == NULL) {
		
			Log_Debug("Could not allocate enough memory for variable length option.\n");

			return -1;

		}

		memcpy(msg->VAROPTIONS, byteSource + delimLoc[1] + 1, length - 1);

		msg->VAROPTIONS[length - 1] = 0;

		msg->varied = 1;

		returner = 4;
	
	}	
	

	return returner;
}


	
	/*
	int length = 0;

	if (maxlen < 30) return -1;

	int delim = strlen(msg->COMMAND);

	memcpy(Array, msg->COMMAND, delim);

	memcpy(Array + delim, msg->DELIMMITER, 1);	

	delim+= 1;

	int newl = strlen(msg->ARGUMENTS);

	memcpy(Array + delim, msg->ARGUMENTS, newl);

	memcpy(Array + delim + newl, msg->DELIMMITER, 1);

	memcpy(Array + delim + newl + 1, msg->OPTIONS, strlen(msg->OPTIONS));

	return length;
*/