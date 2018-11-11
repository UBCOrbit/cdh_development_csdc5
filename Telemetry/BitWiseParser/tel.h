#ifndef TEL
#define TEL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#define SIZE 4

#define CDH 0
#define COMMS 1
#define STRUC 2
#define ADSC 3

#pragma pack(1)
struct data{
	uint8_t ID;
	uint16_t value;
	uint8_t check;
	uint32_t time; 
};

char* reportString(struct data);
struct data constructor(uint8_t a[]);
struct data receiveData(uint16_t);
uint32_t getTime();
void sendData(struct data);
uint16_t str2int(char[], uint8_t);

#endif