#include "TRILLIUM_SPI.h"

/**
Purpose: Switches Pin 8 and 9 from input to output or visa versa depending
on the passed number
Accepts: int i, if i = 0 switches pins to input, otherwise switches to output
**/
void Switch_IO(int i){
    uint32_t temp = 0x00U;

    if(i){

        temp = GPIOA->MODER;
        temp &= ~(GPIO_MODER_MODER0 << (9 * 2U));
        temp &= ~(GPIO_MODER_MODER0 << (8 * 2U));
        temp |= ((GPIO_MODE_OUTPUT_PP & GPIO_MODE) << (9 * 2U));
        temp |= ((GPIO_MODE_OUTPUT_PP & GPIO_MODE) << (8 * 2U));
        GPIOA->MODER = temp;

    }else{

	 	temp = GPIOA->MODER;
	 	temp &= ~(GPIO_MODER_MODER0 << (9 * 2U));
	 	temp &= ~(GPIO_MODER_MODER0 << (8 * 2U));
	 	temp |= ((GPIO_MODE_INPUT & GPIO_MODE) << (9 * 2U));
	 	temp |= ((GPIO_MODE_INPUT & GPIO_MODE) << (8 * 2U));
	 	GPIOA->MODER = temp;

    }

}

/**
Purpose: Sets Pins 8 and 9 low causeing the MCUs connected
to go into slave mode
**/
void Set_Slaves(){

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

}

/**
Purpose: Sets pins 8 and 9 high causing the MCUs connected
to have the capability to enter into master mode
**/
void Set_Masters(){

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

}

/**
Purpose: Clears the MCUs master bit in the register allowing
it to enter back into master mode
**/
void Clear_Master_Register(){

	READ_REG(hspi2.Instance->SR);
	uint16_t regCR1 = READ_REG(hspi2.Instance -> CR1);
    WRITE_REG(hspi2.Instance -> CR1, ( regCR1| (1 << 2)));

}

/**
Purpose: Causes the LED on pin 5 to blink x number
of times with half second delays
Accepts: An integer x indicating the number of blinks
**/
void Blink(int x){

    for(int i = 0; i < x; i ++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

		HAL_Delay(500);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

		HAL_Delay(500);
	}

}