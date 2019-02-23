/*
 * File: rtc_health_check.c
 * Description: Library for the rtc health check feature
 * Created: 2019/02/02
 * Updated: 2019/02/16
 */

#include "rtc_health_check.h"

/**
  * @brief  This function is called to perform the rtc health check calculations
  * @param	hrtc:		    Pointer to a RTC type
  * @param  huart1:       	Pointer to UART connected to first MCU
  * @param  huart2:         Pointer to UART connected to second MCU
  * @param  sTimeStamp:		Pointer to a time structure for the timestamp
  * @param  sTimeStampDate:	Pointer to a date structure for the timestamp
  * @retval HAL status
  */
HAL_StatusTypeDef rtc_health_check(RTC_HandleTypeDef *hrtc, UART_HandleTypeDef *huart1, UART_HandleTypeDef *huart2, RTC_TimeTypeDef *sTime) {

    RTC_TimeTypeDef     sTimeStamp;
    RTC_DateTypeDef     sTimeStampDate;
    RTC_TimeTypeDef     sTimeStampBuffer1;
    RTC_TimeTypeDef     sTimeStampBuffer2;
    int8_t              diff_hr[2] = {0};
    int8_t              diff_mn[2] = {0};
    int8_t              diff_sc[2] = {0};
    _Bool                diff_mcu[2] = {0};
    _Bool                diff_all_mcu = 0;

    ///////////////////////////////////////////
    // Check for timestamp flag? TSF in RTC_ISR
    ///////////////////////////////////////////
    if (!(RTC->ISR & RTC_FLAG_TSF))
    	return HAL_ERROR;

    // Store timestamp in buffer and clear timestamp flags (TSF AND TSOVF)
    // HAL_StatusTypeDef HAL_RTCEx_GetTimeStamp(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef* sTimeStamp, RTC_DateTypeDef* sTimeStampDate, uint32_t Format)
    HAL_RTCEx_GetTimeStamp(hrtc, &sTimeStamp, &sTimeStampDate, RTC_FORMAT_BIN);

	__HAL_RTC_TIMESTAMP_CLEAR_FLAG(hrtc, RTC_FLAG_TSF);
	__HAL_RTC_TIMESTAMP_CLEAR_FLAG(hrtc, RTC_FLAG_TSOVF);

    /////////////////////////////////////
    // Transmit timestamp to other 2 MCUs
    /////////////////////////////////////
    HAL_UART_Transmit(huart1, &sTimeStamp, sizeof(RTC_TimeTypeDef), 0xFFFF);
    HAL_UART_Transmit(huart2, &sTimeStamp, sizeof(RTC_TimeTypeDef), 0xFFFF);

    // Poll and read timestamp from other 2 MCUs
    while( HAL_UART_Receive(huart1, &sTimeStampBuffer1, sizeof(RTC_TimeTypeDef), 0xFFFF) ) {};
    while( HAL_UART_Receive(huart2, &sTimeStampBuffer2, sizeof(RTC_TimeTypeDef), 0xFFFF) ) {};

    /////////////////////
    // Compare timestamps
    /////////////////////
    //  Hours
    diff_hr[0] = ((uint16_t) sTimeStamp.Hours) - ((uint16_t) sTimeStampBuffer1.Hours);
    diff_hr[1] = ((uint16_t) sTimeStamp.Hours) - ((uint16_t) sTimeStampBuffer2.Hours);

    //  Minutes
    diff_mn[0] = ((uint16_t) sTimeStamp.Minutes) - ((uint16_t) sTimeStampBuffer1.Minutes);
    diff_mn[1] = ((uint16_t) sTimeStamp.Minutes) - ((uint16_t) sTimeStampBuffer2.Minutes);

    //  Seconds
    diff_sc[0] = ((uint16_t) sTimeStamp.Seconds) - ((uint16_t) sTimeStampBuffer1.Seconds);
    diff_sc[1] = ((uint16_t) sTimeStamp.Seconds) - ((uint16_t) sTimeStampBuffer2.Seconds);

    diff_mcu[0] = diff_hr[0] | diff_mn[0] | diff_sc[0];
    diff_mcu[1] = diff_hr[1] | diff_mn[1] | diff_sc[1];
    diff_all_mcu = !( (diff_hr[0] && diff_mcu[1]) && (diff_mn[0] && diff_mn[1]) && (diff_sc[0] && diff_sc[1]) );

    ///////////////////
    // Make corrections
    ///////////////////
    //  If the respective MCU’s timestamp matches one other timestamp, no adjustment is performed to this MCU’s RTC
    if (!(diff_mcu[0] && diff_mcu[1]))
        return HAL_OK;

    //  If all timestamps are different, the middle value will be used to perform RTC time adjustment
    if (diff_all_mcu) {
        // Set time as average of all three
        sTime->Hours =   uint8_avg_three(sTime->Hours, sTimeStampBuffer1.Hours, sTimeStampBuffer2.Hours);
        sTime->Minutes = uint8_avg_three(sTime->Minutes, sTimeStampBuffer1.Minutes, sTimeStampBuffer2.Minutes);
        sTime->Seconds = uint8_avg_three(sTime->Seconds, sTimeStampBuffer1.Seconds, sTimeStampBuffer2.Seconds);
    }

    //  If the respective MCU’s timestamp is different from the other two, it shall adjust its RTC’s time using 
    //  Hope it takes less than a second for this entire thing to run
    else if (diff_mcu[0]) {
        sTime->Hours   -= diff_hr[0];
        sTime->Minutes -= diff_mn[0];
        sTime->Seconds -= diff_sc[0];
    }
    else {
        sTime->Hours   -= diff_hr[1];
        sTime->Minutes -= diff_mn[1];
        sTime->Seconds -= diff_sc[1];
    }

    // HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
    return HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BIN);

}

/**
  * @brief  This function is called to send a signal to the timestamp pins on all MCUs
  * @param	pGPIO:		    Pointer to GPIO that's connected to timestamp pins
  * @param  GPIO_pin:       Pin # for GPIO connected to timestamp pins
  * @retval HAL status
  */
HAL_StatusTypeDef rtc_health_check_request(GPIO_TypeDef *pGPIO, uint16_t GPIO_pin) {

    // Config GPIO pin from input to output
    pGPIO->MODER &= (0x00 << (GPIO_pin*2));
    pGPIO->MODER |= (0x01 << (GPIO_pin*2));

    // Set GPIO to high to set timestamp on all MCUs
    HAL_GPIO_WritePin(pGPIO, GPIO_pin, GPIO_PIN_SET);

    // Config GPIO pin back to intput from output
    pGPIO->MODER &= (0x00 << (GPIO_pin*2));

    // Repeat for round 2? Or just call twice
    return HAL_OK;

}

/**
  * @brief  This function subtracts two 8bit unsigned ints and returns the aboslute value
  * @param	x:		        uint8_t
  * @param  y:              uint8_t
  * @retval abs(x-y)
  */
uint8_t uint8_subtract(uint8_t x, uint8_t y) {
    if (x > y)
        return (x-y);
    return (y-x);

}

/**
  * @brief  This function takes the average of three values
  * @param	x:		        uint8_t
  * @param  y:              uint8_t
  * @param  z:              uint8_t
  * @retval avg(x-y)
  */
uint8_t uint8_avg_three(uint8_t x, uint8_t y, uint8_t z) {
    return (x+y+z)/3;
}
