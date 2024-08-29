/*
 * myVolatile.c
 *
 *  Created on: 2023年6月27日
 *      Author: dell
 */

#include "show.h"

volatile uint8_t tempArray[7] = {1, 1, 1, 1, 1, 1, 0};		// 暂存设置的年月日时分秒
volatile uint8_t tempArrayAlarmClock[6] = {0, 0, 0, 0, 0, 0};		// 设置闹钟初始值
volatile int musicId = 1;		// 音乐ID
volatile uint8_t flagAMPM = 0;		// AM/PM标志
volatile uint8_t flagBluetooth = 255;		// 蓝牙命令标志
volatile uint8_t flagWeather = 0;
volatile uint8_t flagTemp = 25;
volatile uint8_t flagHumi = 65;
volatile uint8_t flagRun = 0;
volatile uint8_t flagMonth = 0;
volatile uint8_t flagDay = 0;
volatile uint8_t flagSendMessageForFireAlarm = 0;

void loadSettingOrignalValue(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);

	tempArray[0] = RTC_DateStructure.Year;
	tempArray[1] = RTC_DateStructure.Month;
	tempArray[2] = RTC_DateStructure.Date;
	tempArray[3] = RTC_TimeStructure.Hours;
	tempArray[4] = RTC_TimeStructure.Minutes;
	tempArray[5] = RTC_TimeStructure.Seconds;
	tempArray[6] = RTC_DateStructure.WeekDay;
}

uint8_t returnFlagBluetooth(void)
{
	return flagBluetooth;
}

void clearBluetoothCommand(void)
{
	flagBluetooth = 255;
}

void showWeather()
{
	if(flagWeather==1){oled_show_china(0,40,71, SSD1306_COLOR_WHITE);}       //晴
	if(flagWeather==2){oled_show_china(0,40,72, SSD1306_COLOR_WHITE);}       //阴
	if(flagWeather==3){oled_show_china(0,40,73, SSD1306_COLOR_WHITE);oled_show_china(20,40,74, SSD1306_COLOR_WHITE);}    //多云
	if(flagWeather==4){oled_show_china(0,40,75, SSD1306_COLOR_WHITE);}       //雾
	if(flagWeather==5){oled_show_china(0,40,76, SSD1306_COLOR_WHITE);}       //霾
}

void showMonth()
{
	switch(flagMonth)
	{
	case 1:
		oled_show_china(20,0,49, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);     //月
		break;
	case 2:
		oled_show_china(20,0,40, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 3:
		oled_show_china(20,0,41, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 4:
		oled_show_china(20,0,42, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 5:
		oled_show_china(20,0,43, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 6:
		oled_show_china(20,0,44, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 7:
		oled_show_china(20,0,45, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 8:
		oled_show_china(20,0,46, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 9:
		oled_show_china(20,0,47, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 10:
		oled_show_china(20,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 11:
		oled_show_china(20,0,30, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	case 12:
		oled_show_china(20,0,50, SSD1306_COLOR_WHITE);
		oled_show_china(40,0,51, SSD1306_COLOR_WHITE);
		break;
	default:
		break;
	}
}

void showDay()
{
	switch(flagDay)
	{
	case 1:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,39, SSD1306_COLOR_WHITE);
		break;
	case 2:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,40, SSD1306_COLOR_WHITE);
		break;
	case 3:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,41, SSD1306_COLOR_WHITE);
		break;
	case 4:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,42, SSD1306_COLOR_WHITE);
		break;
	case 5:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,43, SSD1306_COLOR_WHITE);
		break;
	case 6:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,44, SSD1306_COLOR_WHITE);
		break;
	case 7:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,45, SSD1306_COLOR_WHITE);
		break;
	case 8:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,46, SSD1306_COLOR_WHITE);
		break;
	case 9:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,47, SSD1306_COLOR_WHITE);
		break;
	case 10:
		oled_show_china(60,0,52, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,48, SSD1306_COLOR_WHITE);
		break;
	case 11:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,39, SSD1306_COLOR_WHITE);
		break;
	case 12:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,40, SSD1306_COLOR_WHITE);
		break;
	case 13:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,41, SSD1306_COLOR_WHITE);
		break;
	case 14:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,42, SSD1306_COLOR_WHITE);
		break;
	case 15:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,43, SSD1306_COLOR_WHITE);
		break;
	case 16:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,44, SSD1306_COLOR_WHITE);
		break;
	case 17:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,45, SSD1306_COLOR_WHITE);
		break;
	case 18:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,46, SSD1306_COLOR_WHITE);
		break;
	case 19:
		oled_show_china(60,0,48, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,47, SSD1306_COLOR_WHITE);
		break;
	case 20:
		oled_show_china(60,0,40, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,48, SSD1306_COLOR_WHITE);
		break;
	case 21:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,39, SSD1306_COLOR_WHITE);
		break;
	case 22:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,40, SSD1306_COLOR_WHITE);
		break;
	case 23:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,41, SSD1306_COLOR_WHITE);
		break;
	case 24:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,42, SSD1306_COLOR_WHITE);
		break;
	case 25:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,43, SSD1306_COLOR_WHITE);
		break;
	case 26:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,44, SSD1306_COLOR_WHITE);
		break;
	case 27:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,45, SSD1306_COLOR_WHITE);
		break;
	case 28:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,46, SSD1306_COLOR_WHITE);
		break;
	case 29:
		oled_show_china(60,0,53, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,47, SSD1306_COLOR_WHITE);
		break;
	case 30:
		oled_show_china(60,0,41, SSD1306_COLOR_WHITE);
		oled_show_china(80,0,48, SSD1306_COLOR_WHITE);
		break;
	default:
		break;
	}
}
