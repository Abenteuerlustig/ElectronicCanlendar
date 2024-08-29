/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "rtc.h"
#include "led.h"
#include "key.h"
#include "myVolatile.h"

uint8_t Uart2_aRxBuffer;				// 当前接收
uint8_t Uart2_RxBuffCommon[21];		// 接收中断缓冲（普通蓝牙命令）
uint8_t Uart2_RxBuffLunarCanlender[21];		// 接收中断缓冲（农历）
uint8_t Uart2_RxBuffWeather[21];		// 接收中断缓冲（天气、温湿度）
uint8_t Uart2_RxBuffTime[21];		// 实时时间日期
uint8_t Uart2_Rx_Cnt = 0;				// 接收缓冲计数

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  HAL_UART_Receive_IT(&huart2, (uint8_t *)&Uart2_aRxBuffer, 1);   // 启用USART2串口的接收中断

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = BLUETOOTH_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BLUETOOTH_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BLUETOOTH_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BLUETOOTH_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, BLUETOOTH_TX_Pin|BLUETOOTH_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// 中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t tempCommon = 'a';
    uint8_t tempWeather = 'b';
    uint8_t tempLunarCanlender = 'c';
    uint8_t tempTime = 'd';

    LED_SET_ON;		// 蓝牙发送数据指示灯

    if(huart ->Instance == USART2)	// huart==&huart2 两种判断条件等价
    {

		Uart2_RxBuffCommon[Uart2_Rx_Cnt] = Uart2_aRxBuffer;
		Uart2_RxBuffLunarCanlender[Uart2_Rx_Cnt] = Uart2_aRxBuffer;
		Uart2_RxBuffWeather[Uart2_Rx_Cnt] = Uart2_aRxBuffer;
		Uart2_RxBuffTime[Uart2_Rx_Cnt] = Uart2_aRxBuffer;
		Uart2_Rx_Cnt++;

		// 普通蓝牙命令
		if((Uart2_RxBuffCommon[Uart2_Rx_Cnt - 1] == tempCommon) && (Uart2_RxBuffCommon[Uart2_Rx_Cnt - 2] == tempCommon))	// 判断是否为命令结束标志
		{

			// 处理蓝牙命令
			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 0;
			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 1;
			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '2'))	flagBluetooth = 2;
			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '3'))	flagBluetooth = 3;
			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '4'))	flagBluetooth = 4;
			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 5;
			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 6;
			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '2'))	flagBluetooth = 7;
			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '3'))	flagBluetooth = 8;
			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '4'))	flagBluetooth = 9;
			if((Uart2_RxBuffCommon[0] == '3') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 10;
			if((Uart2_RxBuffCommon[0] == '4') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 11;
			if((Uart2_RxBuffCommon[0] == '5') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 16;
			if((Uart2_RxBuffCommon[0] == '5') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 17;
			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 18;
			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 19;
			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '2'))	flagBluetooth = 20;
			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '3'))	flagBluetooth = 21;		// 开始播放音乐、暂停播放音乐
			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '4'))	flagBluetooth = 22;

			// 回显检测
//			HAL_UART_Transmit(&huart1, (uint8_t *)Uart2_RxBuffCommon, 2, 1000);
//			HAL_UART_Transmit(&huart1, (uint8_t *)&flagBluetooth, 1, 1000);

			Uart2_Rx_Cnt = 0;		// 清空计数器
			memset(Uart2_RxBuffCommon,0x00,sizeof(Uart2_RxBuffCommon));		// 清空缓冲数组
			memset(Uart2_RxBuffLunarCanlender,0x00,sizeof(Uart2_RxBuffLunarCanlender));		// 清空缓冲数组
			memset(Uart2_RxBuffWeather,0x00,sizeof(Uart2_RxBuffWeather));		// 清空缓冲数组
			memset(Uart2_RxBuffTime,0x00,sizeof(Uart2_RxBuffTime));		// 清空缓冲数组
		}

		// 天气、温湿度
		if((Uart2_RxBuffWeather[Uart2_Rx_Cnt - 1] == tempWeather) && (Uart2_RxBuffWeather[Uart2_Rx_Cnt - 2] == tempWeather))
		{
//			memccpy(huart_dataWeather,Uart2_RxBuffWeather,sizeof(huart_dataWeather),sizeof(Uart2_RxBuffWeather));	// 将收到的信息存到数组 huart_dataWeather 中

			// 处理蓝牙命令
			HandleBuleToothWeather(Uart2_RxBuffWeather[0]);		// 天气标志
			HandleBuleToothTemp(Uart2_RxBuffWeather[1], Uart2_RxBuffWeather[2]);		// 温度值
			HandleBuleToothHumi(Uart2_RxBuffWeather[3], Uart2_RxBuffWeather[4]);		// 湿度值

			Uart2_Rx_Cnt = 0;		// 清空计数器
			memset(Uart2_RxBuffCommon,0x00,sizeof(Uart2_RxBuffCommon));		// 清空缓冲数组
			memset(Uart2_RxBuffLunarCanlender,0x00,sizeof(Uart2_RxBuffLunarCanlender));		// 清空缓冲数组
			memset(Uart2_RxBuffWeather,0x00,sizeof(Uart2_RxBuffWeather));		// 清空缓冲数组
			memset(Uart2_RxBuffTime,0x00,sizeof(Uart2_RxBuffTime));		// 清空缓冲数组
		}

		// 农历
		if((Uart2_RxBuffLunarCanlender[Uart2_Rx_Cnt - 1] == tempLunarCanlender) && (Uart2_RxBuffLunarCanlender[Uart2_Rx_Cnt - 2] == tempLunarCanlender))
		{
			// 处理蓝牙命令
			HandleBuleToothRun(Uart2_RxBuffLunarCanlender[0],Uart2_RxBuffLunarCanlender[1],Uart2_RxBuffLunarCanlender[2],Uart2_RxBuffLunarCanlender[3]);		// 闰
			HandleBuleToothMonth(Uart2_RxBuffLunarCanlender[2],Uart2_RxBuffLunarCanlender[3]);		// 月
			HandleBuleToothDay(Uart2_RxBuffLunarCanlender[4],Uart2_RxBuffLunarCanlender[5]);		// 日

			HAL_UART_Transmit(&huart1, (uint8_t *)Uart2_RxBuffLunarCanlender, 6, 1000);		// 回显检测

			Uart2_Rx_Cnt = 0;		// 清空计数器
			memset(Uart2_RxBuffCommon,0x00,sizeof(Uart2_RxBuffCommon));		// 清空缓冲数组
			memset(Uart2_RxBuffLunarCanlender,0x00,sizeof(Uart2_RxBuffLunarCanlender));		// 清空缓冲数组
			memset(Uart2_RxBuffWeather,0x00,sizeof(Uart2_RxBuffWeather));		// 清空缓冲数组
			memset(Uart2_RxBuffTime,0x00,sizeof(Uart2_RxBuffTime));		// 清空缓冲数组
		}

		// 实时时间日期
		if((Uart2_RxBuffTime[Uart2_Rx_Cnt - 1] == tempTime) && (Uart2_RxBuffTime[Uart2_Rx_Cnt - 2] == tempTime))
		{
			// 处理蓝牙命令
			tempArray[0] = (Uart2_RxBuffTime[2] - 48) * 10 + (Uart2_RxBuffTime[3] - 48);		// 年
			tempArray[1] = (Uart2_RxBuffTime[5] - 48) * 10 + (Uart2_RxBuffTime[6] - 48);		// 月
			tempArray[2] = (Uart2_RxBuffTime[8] - 48) * 10 + (Uart2_RxBuffTime[9] - 48);		// 日
			tempArray[3] = (Uart2_RxBuffTime[11] - 48) * 10 + (Uart2_RxBuffTime[12] - 48);		// 时
			tempArray[4] = (Uart2_RxBuffTime[14] - 48) * 10 + (Uart2_RxBuffTime[15] - 48);		// 分
			tempArray[5] = (Uart2_RxBuffTime[17] - 48) * 10 + (Uart2_RxBuffTime[18] - 48);		// 秒

			HAL_UART_Transmit(&huart1, (uint8_t *)Uart2_RxBuffTime, 19, 1000);		// 回显检测

			// 同步万年历时间日期
			RTC_TimeTypeDef RTC_TimeStructure = {tempArray[3],tempArray[4],tempArray[5]};
			RTC_DateTypeDef RTC_DateStructure = {0, tempArray[1],tempArray[2],tempArray[0]};
			HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
			HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
			HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
			HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
			HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);

			HAL_UART_Transmit(&huart1, (uint8_t *)Uart2_RxBuffTime, 19, 1000);		// 回显检测

			Uart2_Rx_Cnt = 0;		// 清空计数器
			memset(Uart2_RxBuffCommon,0x00,sizeof(Uart2_RxBuffCommon));		// 清空缓冲数组
			memset(Uart2_RxBuffLunarCanlender,0x00,sizeof(Uart2_RxBuffLunarCanlender));		// 清空缓冲数组
			memset(Uart2_RxBuffWeather,0x00,sizeof(Uart2_RxBuffWeather));		// 清空缓冲数组
			memset(Uart2_RxBuffTime,0x00,sizeof(Uart2_RxBuffTime));		// 清空缓冲数组
		}
	}

	HAL_UART_Receive_IT(&huart2, (uint8_t *)&Uart2_aRxBuffer, 1);   // 重新启用USART2串口的接收中断
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    uint8_t temp = 'a';
//
//    LED_SET_ON;
//
//    if(huart ->Instance == USART2)	// huart==&huart2 两种判断条件等价
//    {
//		Uart2_RxBuffCommon[Uart2_Rx_Cnt++] = Uart2_aRxBuffer;
//		if((Uart2_RxBuffCommon[Uart2_Rx_Cnt - 1] == temp) && (Uart2_RxBuffCommon[Uart2_Rx_Cnt - 2] == temp))	// 判断是否为命令结束标志
//		{
////			memccpy(huart_dataCommon,Uart2_RxBuffCommon,sizeof(huart_dataCommon),sizeof(Uart2_RxBuffCommon));	// 将收到的信息存到数组 huart_dataCommon 中
////
////			// 将收到的信息存到数组 huart_dataCommon 中，和上一句代码作用相同
////			for(uint8_t i = 0; i < SIZEARRAY; i++)
////			{
////				huart_dataCommon[i] = Uart2_RxBuffCommon[i];
////			}
//
//			// 处理蓝牙命令
//			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 0;
//			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 1;
//			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '2'))	flagBluetooth = 2;
//			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '3'))	flagBluetooth = 3;
//			if((Uart2_RxBuffCommon[0] == '1') && (Uart2_RxBuffCommon[1] == '4'))	flagBluetooth = 4;
//			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 5;
//			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 6;
//			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '2'))	flagBluetooth = 7;
//			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '3'))	flagBluetooth = 8;
//			if((Uart2_RxBuffCommon[0] == '2') && (Uart2_RxBuffCommon[1] == '4'))	flagBluetooth = 9;
//			if((Uart2_RxBuffCommon[0] == '3') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 10;
//			if((Uart2_RxBuffCommon[0] == '4') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 11;
//			if((Uart2_RxBuffCommon[0] == '5') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 16;
//			if((Uart2_RxBuffCommon[0] == '5') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 17;
//			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '0'))	flagBluetooth = 18;
//			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '1'))	flagBluetooth = 19;
//			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '2'))	flagBluetooth = 20;
//			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '3'))	flagBluetooth = 21;		// 开始播放音乐、暂停播放音乐
//			if((Uart2_RxBuffCommon[0] == '6') && (Uart2_RxBuffCommon[1] == '4'))	flagBluetooth = 22;
//
//			// 回显检测
//			HAL_UART_Transmit(&huart1, (uint8_t *)Uart2_RxBuffCommon, 2, 1000);
//			HAL_UART_Transmit(&huart1, (uint8_t *)&flagBluetooth, 1, 1000);
//
//			Uart2_Rx_Cnt = 0;		// 清空计数器
//			memset(Uart2_RxBuffCommon,0x00,sizeof(Uart2_RxBuffCommon));		// 清空缓冲数组
//		}
//	}
//
//	HAL_UART_Receive_IT(&huart2, (uint8_t *)&Uart2_aRxBuffer, 1);   // 重新启用USART2串口的接收中断
//}

// 发送字符串
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
	do
	{
		HAL_UART_Transmit(&huart1,(uint8_t *)(str + k) ,1,1000);
		k++;
	} while(*(str + k)!='\0');
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发一字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);

	return (ch);
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);
	return (ch);
}

void HandleBuleToothWeather(uint8_t weather)
{
	if(weather == '1'){flagWeather=1;}
	if(weather == '2'){flagWeather=2;}
	if(weather == '3'){flagWeather=3;}
	if(weather == '4'){flagWeather=4;}
	if(weather == '5'){flagWeather=5;}
}

void HandleBuleToothTemp(uint8_t temp1, uint8_t temp2)
{
	flagTemp = (temp1 - 48) * 10 + (temp2 - 48);
}

void HandleBuleToothHumi(uint8_t temp1, uint8_t temp2)
{
	flagHumi = (temp1 - 48) * 10 + (temp2 - 48);

}

void HandleBuleToothRun(uint8_t temp1, uint8_t temp2, uint8_t temp3, uint8_t temp4)
{
	uint8_t temp = (temp1 - 48) * 10 + (temp2 - 48);
	HandleBuleToothMonth(temp3, temp4);
	if(temp != flagMonth)
	{
		flagRun = 0;
	}
	else
	{
		flagRun = 1;
	}
}

void HandleBuleToothMonth(uint8_t temp1, uint8_t temp2)
{
	if((temp1=='0') && (temp2=='1')){flagMonth=1;}
	if((temp1=='0') && (temp2=='2')){flagMonth=2;}
	if((temp1=='0') && (temp2=='3')){flagMonth=3;}
	if((temp1=='0') && (temp2=='4')){flagMonth=4;}
	if((temp1=='0') && (temp2=='5')){flagMonth=5;}
	if((temp1=='0') && (temp2=='6')){flagMonth=6;}
	if((temp1=='0') && (temp2=='7')){flagMonth=7;}
	if((temp1=='0') && (temp2=='8')){flagMonth=8;}
	if((temp1=='0') && (temp2=='9')){flagMonth=9;}
	if((temp1=='1') && (temp2=='0')){flagMonth=10;}
	if((temp1=='1') && (temp2=='1')){flagMonth=11;}
	if((temp1=='1') && (temp2=='2')){flagMonth=12;}
}

void HandleBuleToothDay(uint8_t temp1, uint8_t temp2)
{
	if((temp1=='0') && (temp2=='1')){flagDay=1;}
	if((temp1=='0') && (temp2=='2')){flagDay=2;}
	if((temp1=='0') && (temp2=='3')){flagDay=3;}
	if((temp1=='0') && (temp2=='4')){flagDay=4;}
	if((temp1=='0') && (temp2=='5')){flagDay=5;}
	if((temp1=='0') && (temp2=='6')){flagDay=6;}
	if((temp1=='0') && (temp2=='7')){flagDay=7;}
	if((temp1=='0') && (temp2=='8')){flagDay=8;}
	if((temp1=='0') && (temp2=='9')){flagDay=9;}
	if((temp1=='1') && (temp2=='0')){flagDay=10;}
	if((temp1=='1') && (temp2=='1')){flagDay=11;}
	if((temp1=='1') && (temp2=='2')){flagDay=12;}
	if((temp1=='1') && (temp2=='3')){flagDay=13;}
	if((temp1=='1') && (temp2=='4')){flagDay=14;}
	if((temp1=='1') && (temp2=='5')){flagDay=15;}
	if((temp1=='1') && (temp2=='6')){flagDay=16;}
	if((temp1=='1') && (temp2=='7')){flagDay=17;}
	if((temp1=='1') && (temp2=='8')){flagDay=18;}
	if((temp1=='1') && (temp2=='9')){flagDay=19;}
	if((temp1=='2') && (temp2=='0')){flagDay=20;}
	if((temp1=='2') && (temp2=='1')){flagDay=21;}
	if((temp1=='2') && (temp2=='2')){flagDay=22;}
	if((temp1=='2') && (temp2=='3')){flagDay=23;}
	if((temp1=='2') && (temp2=='4')){flagDay=24;}
	if((temp1=='2') && (temp2=='5')){flagDay=25;}
	if((temp1=='2') && (temp2=='6')){flagDay=26;}
	if((temp1=='2') && (temp2=='7')){flagDay=27;}
	if((temp1=='2') && (temp2=='8')){flagDay=28;}
	if((temp1=='2') && (temp2=='9')){flagDay=29;}
	if((temp1=='3') && (temp2=='0')){flagDay=30;}
}

/* USER CODE END 1 */
