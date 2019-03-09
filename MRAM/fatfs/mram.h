/*
 * mram.h
 *
 * Created: 31 March 2018
 * Author:  Andrada Zoltan
 *
 *     Driver for the MR20H40 magnetic RAM.
 */

#ifndef MRAM_H_
#define MRAM_H_

#include "stm32f4xx_hal.h"

//Memory Commands
#define WRITE_ENABLE     0x06
#define WRITE_DISABLE    0x04
#define READ_STATUS_REG  0x05
#define WRITE_STATUS_REG 0x01
#define READ_DATA        0x03
#define WRITE_DATA       0x02
#define SLEEP_MODE       0xB9
#define ACTIVE_MODE      0xAB

//Memory Protection Codes
#define PROTECT_ALL 0x8E
#define PROTECT_UPPER_HALF 0x8A
#define PROTECT_UPPER_QUARTER 0x86
#define PROTECT_NONE 0x82

//Function prototypes
void init_mem();
void write_enable(int enable);
void write_status(uint8_t *data);
void read_status(uint8_t* status);
void read_mem(uint32_t address, int size, uint8_t *buffer);
void write_mem(uint32_t address, int size, uint8_t *buffer);
void sleep(int sleep);

#endif
