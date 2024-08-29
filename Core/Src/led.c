/*
 * led.c
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#include "led.h"

void LED_Toggle(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
	HAL_Delay(250);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_Delay(250);
}
