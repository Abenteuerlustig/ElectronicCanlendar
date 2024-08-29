/*
 * key.c
 *
 *  Created on: 2023年6月27日
 *      Author: dell
 */

#include "key.h"

// 读取键值
uint8_t readKeyValue(void)
{
	if(KEY1_ON)
	{
		HAL_Delay(20);
		if(KEY1_ON)
		{
			while(KEY1_ON);
			return 1;
		}
		else
		{
			return 0;
		}
	}

	if(KEY2_ON)
	{
		HAL_Delay(20);
		if(KEY2_ON)
		{
			while(KEY2_ON);
			return 2;
		}
		else
		{
			return 0;
		}
	}

	if(KEY3_ON)
	{
		HAL_Delay(20);
		if(KEY3_ON)
		{
			while(KEY3_ON);
			return 3;
		}
		else
		{
			return 0;
		}
	}

	if(KEY4_ON)
	{
		HAL_Delay(20);
		if(KEY4_ON)
		{
			while(KEY4_ON);
			return 4;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}


