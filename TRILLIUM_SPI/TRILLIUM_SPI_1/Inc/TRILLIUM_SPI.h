#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#ifndef TRILLIUM_SPI
#define TRILLIUM_SPI
#define GPIO_MODE             0x00000003U

SPI_HandleTypeDef hspi2;

void Switch_IO(int i);
void Set_Slaves();
void Set_Masters();
void Clear_Master_Register();
void Blink(int x);

#endif
