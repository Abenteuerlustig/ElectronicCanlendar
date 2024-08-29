/*
 * delayUs.c
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#include "delayUs.h"

void delayUs(unsigned int time_us)
{
    HAL_TIM_Base_Start(&htim3);
    __HAL_TIM_SetCounter(&htim3,0);// 先把计数清零
    while( time_us > __HAL_TIM_GetCounter(&htim3) ) ;
    HAL_TIM_Base_Stop(&htim3);
}
