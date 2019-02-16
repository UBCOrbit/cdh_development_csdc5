/*
 * mram.c
 *
 * Created: 31 March 2018
 * Author:  Andrada Zoltan
 *
 *     Driver for the MR20H40 magnetic RAM.
 */

#include "MRAM.h"

SPI_HandleTypeDef hspi2;

/*
 * Initialize the pins to allow for communication to the device.
 */
void init_mem() {
	//HOLD_PIN must be HIGH for any communication with the device
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

	//WP_PIN is LOW to protect writing to Status Register
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

	//CS_PIN back to HIGH to minimize standby power of device
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
* Write enable/disable the MRAM chip. Enabling write will allow for
* memory and status register to be written to; writing doesn't work unless
* this is enabled.
*
* Parameter: enable - 1 if write enabled, 0 if not
*/
void write_enable(int enable) {
	uint8_t cmd;
	if (enable == 1)
		cmd = WRITE_ENABLE;
	else
		cmd = WRITE_DISABLE;

	//Drive CS pin to low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//Send command
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 50);

	//Drive CS pin back to high
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
* Write data to the status register. Function will only successfully
* write if write enable has been executed and ~WP is driven by high.
*
* Parameter: data - 8bit data to write to the status register
*/
void write_status(uint8_t *data) {
	uint8_t cmd = WRITE_STATUS_REG;

	// Drive high to ~WP to write to status register
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

	//Drive CS pin to low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//Send command
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 50);

	//Send data to write to register
	HAL_SPI_Transmit(&hspi2, data, 1, 50);

	// Drive LOW back to ~WP to block writes to status register
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

	//Drive CS pin back to high
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
* Read data from the status register
*
* Parameter: data - 8bit data to write to the status register
*/
void read_status(uint8_t* status) {
	uint8_t cmd = READ_STATUS_REG;

	//Drive CS pin to low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//Send command
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 50);

	// Receive
	while (HAL_SPI_Receive(&hspi2, status, 1, 50) != HAL_OK);

	//Drive CS pin back to high
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
* Read memory at a specified address. Function will continue to read until
* the CS pin is set high.
*
* Parameter: address - 32-bit address to read from, note that MRAM only expects 24-bits
* Parameter: size - size in bytes to read
* Parameter: buffer - pointer in memory where the read data should be stored
*/
void read_mem(uint32_t address, int size, uint8_t *buffer) {
	uint8_t addr[3];
	uint8_t cmd = READ_DATA;

	// ARM processor is little endian, MRAM expects big endian
	addr[0] = (address >> 16) & 0xFF;
	addr[1] = (address >> 8) & 0xFF;
	addr[2] = (address) & 0xFF;

	//Drive CS pin to low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//Send read command
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 50);

	//Send address to read from in memory
	HAL_SPI_Transmit(&hspi2, addr, 3, 50);

	while (HAL_SPI_Receive(&hspi2, buffer, size, 50) != HAL_OK);

	//Drive CS pin back to high to end reading communication
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
* Write data into memory at a specified address.
*
* Parameter: address - 32-bit address to write 2, note that MRAM only expects 24-bits
* Parameter: size - size in bytes to write
* Parameter: buffer - pointer in memory where data to write is stored
*/
void write_mem(uint32_t address, int size, uint8_t *buffer) {
	uint8_t addr[3];
	uint8_t cmd = WRITE_DATA;

	// ARM processor is little endian, MRAM expects big endian
	addr[0] = (address >> 16) & 0xFF;
	addr[1] = (address >> 8) & 0xFF;
	addr[2] = (address) & 0xFF;

	//Drive CS pin to low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//Send write command
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 50);

	//Send address to write to in memory
	HAL_SPI_Transmit(&hspi2, addr, 3, 50);

	//Send data
	HAL_SPI_Transmit(&hspi2, buffer, size, 50);

	//Drive CS pin back to high to end writing communication
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/*
* Command the memory to go into sleep state or come out of a sleep state.
*
* Parameter: sleep - 1 if sleep, 0 if wake up
*/
void sleep(int sleep) {
	uint8_t cmd;
	if (sleep == 1)
		cmd = SLEEP_MODE;
	else
		cmd = ACTIVE_MODE;

	//Drive CS pin to low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	//Send command to wake up or sleep
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 50);

	//Drive CS pin back to high
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(0.4);
}
