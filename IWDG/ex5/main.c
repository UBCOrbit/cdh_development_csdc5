
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
/* USER CODE BEGIN Includes */
#define NUM_STATES 4
#define BITS_PER_TASK 2
//index=task's index in the array, size=bits_per_task
#define GETMASK(index, size) (((1 << (size)) - 1) << (index))
#define READFROM(data, index, size) (((data) & GETMASK((index), (size))) >> (index))
#define WRITETO(data, index, size, value) ((data) = ((data) & (~GETMASK((index), (size)))) | ((value) << (index)))

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;

UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;

//SemaphoreHandle_t xMutex;
//TaskHandle_t sender1 = NULL;
//TaskHandle_t sender2 = NULL;
//TaskHandle_t receiver1 = NULL;
//TaskHandle_t receiver2 = NULL;
//TaskHandle_t highHandle = NULL;
//TaskHandle_t midHandle = NULL;
//TaskHandle_t lowHandle = NULL;
//QueueHandle_t queueHandle;

//unsigned queueSize;
//static uint8_t wd_flag = 0x00U;

//unsigned length = ceil(log(NUM_STATES)/log(2));

typedef enum {
	disabled = 0x00U,
	started = 0x01U,
	waiting = 0x02U,
	finished = 0x03U
} task_state;

typedef struct{
	uint8_t bitflag;
	int index;
	int size;
} taskParams;

//typedef struct {
//	uint8_t value;
//	TaskHandle_t source;
//} sent_data;
//
//typedef struct{
//	uint8_t bit;
//	sent_data data;
//} senderParams;
//
//typedef struct{
//	uint8_t bit;
//} receiverParams;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_IWDG_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);
void demo(taskParams *param);
void demo_2(taskParams *param);
void try(void);
//void task_1(taskParams *param);
//void task_2(taskParams *param);
//void high(void);
//void mid(receiverParams *param);
//void low(senderParams *param);
//void sender_1 (void *parameters);
//void sender_2 (void *parameters);
//void receiver_1 (void *parameters);
//void receiver_2 (void *parameters);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/


/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_IWDG_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  	char str[1000];
  	static uint8_t wd_flag = 0x03U;
  	taskParams demoTask = {0};
  	demoTask.bitflag = wd_flag;
  	demoTask.index = 0;
  	demoTask.size = 2;
//  taskParams demoTask2 = {wd_flag, 2, 2};
    HAL_UART_Transmit(&huart2, str, sprintf(str, "flag: %d\n", demoTask.bitflag), 200);
    WRITETO(demoTask.bitflag, demoTask.index, demoTask.size, started);
    HAL_UART_Transmit(&huart2, str, sprintf(str, "flag: %d\n", demoTask.bitflag), 200);
//  taskParams task1 = {0};
//  taskParams task2 = {0};
//  task1.index = 0x3U;
//  task1.text = "1\n";
//  task2.index = 0xCU;
//  task2.text = "3\n";
  //HAL_UART_Transmit(&huart2, str, sprintf(str, "task1: %d\n", task1.index), 200);
  //HAL_UART_Transmit(&huart2, str, sprintf(str, "task2: %d\n", task2.index), 200);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */
  //sent_data send1 = {0, lowHandle};
  //sent_data send2 = {1, sender2};
  //queueSize = NUM_SENDERS * 2 +1; //add 1 just in case
//  queueSize = 1;
//  queueHandle = xQueueCreate(queueSize, sizeof(sent_data));
//  if(queueHandle != NULL){
////	  xTaskCreate(sender_1, "sender1", 1000, send1, 2, sender1);
////	  xTaskCreate(sender_2, "sender2", 1000, send2, 2, sender2);
////	  xTaskCreate(receiver_1, "receiver1", 1000, NULL, 2, receiver1);
////	  xTaskCreate(receiver_2, "receiver2", 1000, NULL, 2, receiver2);
//	  xTaskCreate(low, "low", 1000, &send1, 1, &lowHandle);
//	  xTaskCreate(mid, "mid", 1000, NULL, 2, &midHandle);
//	  //xTaskCreate(high, "high", 1000, NULL, 3, &highHandle);
//	  vTaskStartScheduler();
//  }
  	xTaskCreate(demo, "demo", 1000, &demoTask, 3, NULL);
//  	  xTaskCreate(demo_2, "demo2", 1000, NULL, 2, NULL);
    //xTaskCreate(try, "try", 1000, NULL, 3, NULL);
//  	  xTaskCreate(task_1, "task_1", 1000, &task1, 2, NULL);
//  	  xTaskCreate(task_2, "task_2", 1000, &task2, 3, NULL);
  	  vTaskStartScheduler();

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
//  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
//  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  //osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */
	  //HAL_UART_Transmit(&huart2, str, sprintf(str, "states: %d \n", NUM_STATES), 1000);
	  //HAL_UART_Transmit(&huart2, str, sprintf(str, "data: %d\n", wd_flag), 1000);
//	  HAL_UART_Transmit(&huart2, str, sprintf(str, "mask: %d\n", GETMASK(0,2)), 1000);
//	  HAL_UART_Transmit(&huart2, str, sprintf(str, "READ: %d\n", WRITETO(data, 0, 2, 2)), 1000);
//	  HAL_Delay(500);
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}
//void try(void){
//	char str[1000];
//	int8_t data = 00000000;
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "before: %d\n", data), 1000);
//	WRITETO(data, 0, 2, started);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "after: %d\n", data), 1000); //worked
//}

void demo(taskParams *param){
	char str[1000];
	HAL_UART_Transmit(&huart2, str, sprintf(str, "task\n"), 200);
	HAL_UART_Transmit(&huart2, str, sprintf(str, "flag: %d\n", param->bitflag), 200);
	WRITETO(param->bitflag, param->index, param->size, started);
	HAL_UART_Transmit(&huart2, str, sprintf(str, "flag: %d\n", param->bitflag), 200);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "flag: %d\n", param->bitflag), 200);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "index: %d\n", param->index), 200);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "size: %d\n", param->size), 200);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "before: %d \n", wd_flag), 1000);
//	WRITETO(wd_flag, 0, 2, started);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "after: %d \n", wd_flag), 1000);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "before: %d \n", param->bitflag), 1000);
//	WRITETO(param->bitflag, param->index, param->size, started);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "after: %d \n", param->bitflag), 1000);
//	WRITETO(param->bitflag, param->index, param->size, finished);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "finish_0: %d \n", param->bitflag), 1000);
	//vTaskDelay(1000);
}
//
//void demo_2(taskParams *param){
//	char str[1000];
//	WRITETO(param->bitflag, param->index, param->size, started);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "start_1: %d \n", param->bitflag), 1000);
//	WRITETO(param->bitflag, param->index, param->size, finished);
//	HAL_UART_Transmit(&huart2, str, sprintf(str, "finish_1: %d \n", param->bitflag), 1000);
//	vTaskDelay(1000);
//}

//void mid(void){
////receiver
//	sent_data data;
//	BaseType_t status;
//	char buffer[100];
//	for(;;){
//		//note: when the sender and receiver tasks are NOT of the same priority, you have to add a blockout time (delay) to the higher priority receiver function because it needs to wait for the lower priority sender function
//		//when they are of the same priority though, this is not a problem because eventually at some time that the data is checked from the receiver, the queue will be loaded (at some other times it will be empty)
//		status=xQueueReceive(queueHandle, &data, 100);
//		if(status == pdPASS){
//			HAL_UART_Transmit(&huart2, buffer, sprintf(buffer, "Received\n"), 100);
//		} else{
//			HAL_UART_Transmit(&huart2, buffer, sprintf(buffer, "Couldn't receive\n"), 100);
//		} HAL_Delay(500); //delay is here just to be able to read the print messages!
//	}
//}
//void low(sent_data *param){
////sender
//	BaseType_t status;
//	char buffer[100];
//	for(;;){
//		status=xQueueSendToBack(queueHandle, param, 100);
//		if(status == pdPASS){
//			HAL_UART_Transmit(&huart2, buffer, sprintf(buffer, "Sent\n"), 100);
//		} else{
//			HAL_UART_Transmit(&huart2, buffer, sprintf(buffer, "Couldn't send\n"), 100);
//		} HAL_Delay(500);
//	}
//}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* IWDG init function */
static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
