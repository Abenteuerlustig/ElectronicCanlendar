/*
 * alarmClock.c
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#include "alarmClock.h"
#include "song.h"

// 闹钟设置入口函数
void extranceAlarmClock(void)
{
	oled_clear();
	setDateAndTimeShowAlarmClock(0);		// 显示装载的初值

	int settingFlag = 1;
	int settedFlag = 0;

	// 标志位不为零时，设置年月日、时分秒
	while((settingFlag < 7) && (settingFlag > 0))
	{
		while(settedFlag == 0)
		{
			// 处理键值
			switch(readKeyValue())
			{
			case 1:		// 当前内容加1
				Increase(settingFlag);
				oled_clear();
				setDateAndTimeShowAlarmClock(settingFlag);
				break;

			case 2:		// 当前内容减1
				Decrease(settingFlag);
				oled_clear();
				setDateAndTimeShowAlarmClock(settingFlag);
				break;

			case 3:		// 确认设置值
				settedFlag = 1;
				break;

			case 4:		// 返回上一级
				settedFlag = 1;
				settingFlag -= 2;
				break;

			default:		// 无操作等待
				oled_clear();
				setDateAndTimeShowAlarmClock(settingFlag);
				break;
			}

			// 处理蓝牙命令
			switch(returnFlagBluetooth())
			{
			case 6:		// 当前内容加1
				Increase(settingFlag);
				oled_clear();
				setDateAndTimeShow(settingFlag);
				clearBluetoothCommand();
				break;

			case 7:		// 当前内容减1
				Decrease(settingFlag);
				oled_clear();
				setDateAndTimeShow(settingFlag);
				clearBluetoothCommand();
				break;

			case 8:		// 确认设置值
				clearBluetoothCommand();
				settedFlag = 1;
				break;

			case 9:		// 返回上一级
				clearBluetoothCommand();
				settedFlag = 1;
				settingFlag -= 2;
				break;

			default:		// 无操作等待
				clearBluetoothCommand();
				oled_clear();
				setDateAndTimeShowAlarmClock(settingFlag);
				break;
			}
		}

		// 修改设置内容指向
		settingFlag++;
		settedFlag = 0;
	}

	if(settingFlag > 0)
	{
		// 选择音乐（闹钟）
		selectSong();

		// 闹钟时间、音乐设定结束后，交换临时变量的值
		for(int i = 0; i < 6; i++)
		{
			tempArrayAlarmClock[i] = tempArray[i];
		}
	}

	oled_clear();
}

// 闹钟模式显示
void setDateAndTimeShowAlarmClock(int settingFlag)
{
	// 闹钟设置（显示）
	oled_show_china(0,0,58, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,59, SSD1306_COLOR_WHITE);
	oled_show_china(32,0,56, SSD1306_COLOR_WHITE);
	oled_show_china(48,0,57, SSD1306_COLOR_WHITE);

	// 基本显示内容
	OLED_ShowNumber(0,20,2,1,8);
	OLED_ShowNumber(10,20,0,1,8);
	if(tempArray[0] > 9)
	{
		OLED_ShowNumber(20,20,tempArray[0],2,16);
	}
	else
	{
		OLED_ShowNumber(20,20,0,1,8);
		OLED_ShowNumber(30,20,tempArray[0],1,8);
	}

	oled_show_char(40, 20,'.', &fontone, SSD1306_COLOR_WHITE);

	if(tempArray[1] > 9)
	{
		OLED_ShowNumber(50,20,tempArray[1],2,16);
	}
	else
	{
		OLED_ShowNumber(50,20,0,1,8);
		OLED_ShowNumber(60,20,tempArray[1],1,8);
	}

	oled_show_char(70, 20,'.', &fontone, SSD1306_COLOR_WHITE);

	if(tempArray[2] > 9)
	{
		OLED_ShowNumber(80,20,tempArray[2],2,16);
	}
	else
	{
		OLED_ShowNumber(80,20,0,1,8);
		OLED_ShowNumber(90,20,tempArray[2],1,8);
	}

	if(tempArray[3] > 9)
	{
		OLED_ShowNumber(20,40,tempArray[3],2,16);
	}
	else
	{
		OLED_ShowNumber(20,40,0,1,8);
		OLED_ShowNumber(30,40,tempArray[3],1,8);
	}

	oled_show_char(40, 40,':', &fontone, SSD1306_COLOR_WHITE);

	if(tempArray[4] > 9)
	{
		OLED_ShowNumber(50,40,tempArray[4],2,16);
	}
	else
	{
		OLED_ShowNumber(50,40,0,1,8);
		OLED_ShowNumber(60,40,tempArray[4],1,8);
	}

	oled_show_char(70, 40,':', &fontone, SSD1306_COLOR_WHITE);

	if(tempArray[5] > 9)
	{
		OLED_ShowNumber(80,40,tempArray[5],2,16);
	}
	else
	{
		OLED_ShowNumber(80,40,0,1,8);
		OLED_ShowNumber(90,40,tempArray[5],1,8);
	}

	oled_update_screen();
}

// 闹钟提示
void alarmClockReminder(void)
{
	// 闹钟提示
	oled_show_china(0,0,58, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,59, SSD1306_COLOR_WHITE);
	oled_show_china(32,0,66, SSD1306_COLOR_WHITE);
	oled_show_china(48,0,67, SSD1306_COLOR_WHITE);

	// 按KEY4关闭闹钟
	oled_show_china(0,20,68, SSD1306_COLOR_WHITE);
	oled_show_str(16,20,"KEY4",&fontone,SSD1306_COLOR_WHITE);
	oled_show_china(48,20,60, SSD1306_COLOR_WHITE);
	oled_show_china(64,20,61, SSD1306_COLOR_WHITE);
	oled_show_china(80,20,58, SSD1306_COLOR_WHITE);
	oled_show_china(96,20,59, SSD1306_COLOR_WHITE);
	oled_update_screen();
}

// 检查闹钟时间到否
void checkAlarmClock(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);

	if((RTC_DateStructure.Year == tempArrayAlarmClock[0]) && (RTC_DateStructure.Month == tempArrayAlarmClock[1]) &&
			(RTC_DateStructure.Date == tempArrayAlarmClock[2]) && (RTC_TimeStructure.Hours == tempArrayAlarmClock[3]) &&
			(RTC_TimeStructure.Minutes == tempArrayAlarmClock[4]) && (RTC_TimeStructure.Seconds == tempArrayAlarmClock[5]))
	{
		alarmClockMusic();
		oled_clear();

		uint8_t keyValue = readKeyValue();
		uint8_t commandFromBluetooth = returnFlagBluetooth();
		clearBluetoothCommand();
		while((keyValue != 4) && (commandFromBluetooth != 10))
		{
			LED_Toggle();		// LED闪烁
			alarmClockReminder();		// 屏幕显示
			keyValue = readKeyValue();
			commandFromBluetooth = returnFlagBluetooth();
			clearBluetoothCommand();
		}

		oled_clear();
		LED_SET_OFF;
		stopMusic();
	}
}
