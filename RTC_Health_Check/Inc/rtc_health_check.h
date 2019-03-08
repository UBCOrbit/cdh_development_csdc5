/* Library for rtc health check */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_HEALTH_CHECK_H__
#define __RTC_HEALTH_CHECK_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private define ------------------------------------------------------------*/


/* Prototypes ----------------------------------------------------------------*/

// Perform rtc health check calculations (should be called from interrupt)
HAL_StatusTypeDef rtc_health_check(RTC_HandleTypeDef *hrtc, UART_HandleTypeDef *huart1, UART_HandleTypeDef *huart2, RTC_TimeTypeDef *sTime);

// Request rtc health check across MCUs by setting timestamps
HAL_StatusTypeDef rtc_health_check_request(GPIO_TypeDef *pGPIO, uint16_t GPIO_pin);

// Take average of three values
uint8_t uint8_avg_three(uint8_t x, uint8_t y, uint8_t z);

// subtract two unsigned ints
// Maybe delete this
uint8_t uint8_subtract(uint8_t x, uint8_t y);

#endif /* __RTC_HEALTH_CHECK_H__ */

