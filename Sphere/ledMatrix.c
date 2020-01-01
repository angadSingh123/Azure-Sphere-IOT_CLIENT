#include "ledMatrix.h"

const struct timespec SETUP_DELAY = { 0, 25 };

const struct timespec CLOCK_DELAY = { 0, 10 };



//convert the column of bits of a 2D array into its 8 bit representation
unsigned char arr2chr(char* array, int column)
{
	unsigned char x = 0b11111111;

	if (*((array)+(1) + column) == 0) x &= 0b10111111;
	if (*((array)+(2) + column) == 0) x &= 0b11011111;
	if (*((array)+(3) + column) == 0) x &= 0b11101111;
	if (*((array)+(4) + column) == 0) x &= 0b11110111;
	if (*((array)+(5) + column) == 0) x &= 0b11111011;
	if (*((array)+(6) + column) == 0) x &= 0b11111101;
	if (*((array)+(7) + column) == 0) x &= 0b11111110;
	if (*((array)+(0) + column) == 0) x &= 0b01111111;

	return x;
}

//store the 8 bits of a char into the specified column of the array
void chr2arr(unsigned char data, char* array, int col) {

	for (int i = 0; i < 8; i++) {

		array[7 - i + col] = (data >> i) & 0b00000001;

	}

}

//shift out 8 bits to the register, least significant bit first
inline void write8(unsigned char* data, char* latchFd, char* dataFd, char* clockFd) {

	GPIO_SetValue(*latchFd, 0);

	for (int i = 0; i < 8; i++) {

		GPIO_SetValue(*dataFd, (*data >> i) & 0b00000001);
		GPIO_SetValue(*clockFd, 1);
		GPIO_SetValue(*clockFd, 0);
	}

	GPIO_SetValue(*latchFd, 1);
}

//shift out 8 bits to the register, most significant bit first
inline void write8_m(unsigned char* data, char* latchFd, char* dataFd, char* clockFd) {

	GPIO_SetValue(*latchFd, 0);

	for (int i = 0; i < 8; i++) {

		GPIO_SetValue(*dataFd, (((*data << i) & 0b10000000) == 128) ? 1 : 0);
		GPIO_SetValue(*clockFd, 1);
		GPIO_SetValue(*clockFd, 0);
	}

	GPIO_SetValue(*latchFd, 1);
}

//shift the data buffer's and the hidden buffer's contents to the right to emulate a scrolling effect
inline void shiftRight(unsigned char* data, unsigned char* buffer) {

	for (int i = 0; i < 8; i++) {

		data[i] >>= 1;

		if ((buffer[i] & 0b00000001) == 1) data[i] = data[i] | 0b10000000;

		buffer[i] >>= 1;
	}

}

//shift the data buffer's and the hidden buffer's contents to the left to emulate a scrolling effect
inline void shiftLeft(unsigned char* data, unsigned char* buffer) {

	for (int i = 0; i < 8; i++) {

		data[i] <<= 1;

		if ((buffer[i] & 0b10000000) == 128) data[i] = data[i] | 0b00000001;

		buffer[i] <<= 1;
	}

}

