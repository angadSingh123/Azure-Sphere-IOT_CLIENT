#include <time.h>
#include <applibs/gpio.h>

#define MSBFIRST 0

#define LSBFIRST 1

unsigned char arr2chr(char* array, int column);

void chr2arr(unsigned char data, char* array, int col);

void write8_m(unsigned char* data, char* latchFd, char* dataFd, char* clockFd);

void write8(unsigned char* data, char* latchFd, char* dataFd, char* clockFd);

void shiftRight(unsigned char* render, unsigned char* buffer);

void shiftLeft(unsigned char* render, unsigned char* buffer);


