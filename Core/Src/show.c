/*
 * show.h
 *
 *  Created on: 2023年6月26日
 *      Author: dell
 */

#include "show.h"

// 设置时间日期入口函数
void extranceDateAndTime(void)
{
	loadSettingOrignalValue();		// 将设置模式装载初值设置为当前时间
	oled_clear();
	setDateAndTimeShow(0);		// 显示装载的初值

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
				setDateAndTimeShow(settingFlag);
				break;

			case 2:		// 当前内容减1
				Decrease(settingFlag);
				oled_clear();
				setDateAndTimeShow(settingFlag);
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
				setDateAndTimeShow(settingFlag);
				break;
			}

			// 处理蓝牙命令
			switch(returnFlagBluetooth())
			{
			case 1:		// 当前内容加1
				Increase(settingFlag);
				oled_clear();
				setDateAndTimeShow(settingFlag);
				clearBluetoothCommand();
				break;

			case 2:		// 当前内容减1
				Decrease(settingFlag);
				oled_clear();
				setDateAndTimeShow(settingFlag);
				clearBluetoothCommand();
				break;

			case 3:		// 确认设置值
				clearBluetoothCommand();
				settedFlag = 1;
				break;

			case 4:		// 返回上一级
				clearBluetoothCommand();
				settedFlag = 1;
				settingFlag -= 2;
				break;

			default:		// 无操作等待
				clearBluetoothCommand();
				oled_clear();
				setDateAndTimeShow(settingFlag);
				break;
			}
		}

		// 修改设置内容指向
		settingFlag++;
		settedFlag = 0;
	}

	if(settingFlag > 0)		// 设置AM/PM
	{
		// 设置AM/PM
		settingAMPM();

		// 设置时间日期
		RTC_TimeTypeDef RTC_TimeStructure = {tempArray[3],tempArray[4],tempArray[5]};
		RTC_DateTypeDef RTC_DateStructure = {0, tempArray[1],tempArray[2],tempArray[0]};
		HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
		HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
	}

	oled_clear();
}

// 设置AM/PM
void settingAMPM(void)
{
	uint8_t keyValueAMPM = readKeyValue();
	uint8_t commandFromBluetooth = returnFlagBluetooth();
	clearBluetoothCommand();
	oled_clear();

	while((keyValueAMPM != 1) && (keyValueAMPM != 2) &&
			(commandFromBluetooth != 16) && (commandFromBluetooth != 17))
	{
		diaplaySettingAMPM();
		keyValueAMPM = readKeyValue();
		commandFromBluetooth = returnFlagBluetooth();
		clearBluetoothCommand();
	}

	// 处理键值
	switch(keyValueAMPM)
	{
	case 1:
		flagAMPM = 1;
		break;

	case 2:
		flagAMPM = 0;
		break;

	default:
		break;
	}

	// 处理蓝牙命令值
	switch(commandFromBluetooth)
	{
	case 16:
		flagAMPM = 1;
		break;

	case 17:
		flagAMPM = 0;
		break;

	default:
		break;
	}

	oled_clear();
}

// 设置AM/PM（显示）
void diaplaySettingAMPM(void)
{
	oled_show_china(0,0,56, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,57, SSD1306_COLOR_WHITE);
	oled_show_str(32, 0,"AM/PM:", &fontone, SSD1306_COLOR_WHITE);
	oled_show_str(0, 20,"KEY1:", &fontone, SSD1306_COLOR_WHITE);
	oled_show_china(40,20,1, SSD1306_COLOR_WHITE);
	oled_show_str(0, 40,"KEY2:", &fontone, SSD1306_COLOR_WHITE);
	oled_show_china(40,40,60, SSD1306_COLOR_WHITE);
	oled_update_screen();
}

// 实时显示设置情况
void setDateAndTimeShow(int settingFlag)
{
	// 设置时间日期（显示）
	oled_show_china(0,0,56, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,57, SSD1306_COLOR_WHITE);
	oled_show_china(32,0,31, SSD1306_COLOR_WHITE);
	oled_show_china(48,0,32, SSD1306_COLOR_WHITE);
	oled_show_china(64,0,64, SSD1306_COLOR_WHITE);
	oled_show_china(80,0,65, SSD1306_COLOR_WHITE);

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

// 加值
void Increase(int settingFlag)
{
	settingFlag -= 1;

	switch(settingFlag)
	{
	case 0:		// 年份合法判断
		if(tempArray[settingFlag] == 99)
		{
			tempArray[settingFlag] = 0;
			return;
		}
		tempArray[settingFlag]++;
		return;

	case 1:		// 月份合法判断
		if(tempArray[settingFlag] == 12)
		{
			if(tempArray[0] == 99){
				tempArray[0] = 0;
			}
			else{
				tempArray[0]++;
			}
			tempArray[settingFlag] = 1;
			return;
		}
		tempArray[settingFlag]++;
		return;

	case 2:		// 日期合法判断
		if(FEBURARY)	// 二月
		{
			if(LEAP_YEAR && (tempArray[settingFlag] == 29))	// 闰年
			{
				tempArray[1]++;
				tempArray[settingFlag] = 1;
				return;
			}
			if(COMMON_YEAR && (tempArray[settingFlag] == 28))	// 平年
			{
				tempArray[1]++;
				tempArray[settingFlag] = 1;
				return;
			}
		}

		if(BIG_MONTH_INC && (tempArray[settingFlag] == 31)){	// 大月

			if((tempArray[1] == 12)){	// 十二月

				if(tempArray[0] == 99){
					tempArray[0] = 0;
				}
				else{
					tempArray[0]++;
				}

				tempArray[1] = 1;
				tempArray[settingFlag] = 1;
				return;
			}
			else{	// 其他
				tempArray[1]++;
				tempArray[settingFlag] = 1;
				return;
			}
		}

		if(SMALL_MONTH_INC && (tempArray[settingFlag] == 30)){	// 小月
			tempArray[1]++;
			tempArray[settingFlag] = 1;
			return;
		}

		tempArray[settingFlag]++;	// 不符合前面的条件，项目值加1
		return;

	case 3:		// 小时合法判断
		if(tempArray[settingFlag] == 23)
		{
			tempArray[settingFlag] = 0;
			return;
		}
		tempArray[settingFlag]++;
		return;

	case 4:		// 分钟合法判断
		if(tempArray[settingFlag] == 59)
		{
			if(tempArray[3] == 23){
				tempArray[3] = 0;
			}
			else{
				tempArray[3]++;
			}
			tempArray[settingFlag] = 0;
			return;
		}
		tempArray[settingFlag]++;
		return;

	case 5:		// 秒钟合法判断
		if(tempArray[settingFlag] == 59)
		{
			if(tempArray[4] == 59){
				if(tempArray[3] == 23){
					tempArray[3] = 0;
					dealCrossingDay(INCREASE);
				}
				else{
					tempArray[3]++;
				}
				tempArray[4] = 0;
			}
			else{
				tempArray[4]++;
			}
			tempArray[settingFlag] = 0;
			return;
		}
		tempArray[settingFlag]++;
		return;

	default:
		return;
	}
}

// 减值
void Decrease(int settingFlag)
{
	settingFlag -= 1;
	switch(settingFlag)
		{
		case 0:		// 年份合法判断
			if(tempArray[settingFlag] == 0)
			{
				tempArray[settingFlag] = 99;
				return;
			}
			tempArray[settingFlag]--;
			return;

		case 1:		// 月份合法判断
			if(tempArray[settingFlag] == 1)
			{
				if(tempArray[0] == 0){
					tempArray[0] = 99;
				}
				else{
					tempArray[0]--;
				}
				tempArray[settingFlag] = 12;
				return;
			}
			tempArray[settingFlag]--;
			return;

		case 2:		// 日期合法判断
			if(MARCH)	// 三月
			{
				if(LEAP_YEAR && (tempArray[settingFlag] == 1))	// 闰年
				{
					tempArray[1]--;
					tempArray[settingFlag] = 29;
					return;
				}
				if(COMMON_YEAR && (tempArray[settingFlag] == 1))	// 平年
				{
					tempArray[1]--;
					tempArray[settingFlag] = 28;
					return;
				}
			}

			if(BIG_MONTH_DEC && (tempArray[settingFlag] == 1)){	// 大月后

				if((tempArray[1] == 1)){	// 十二月后

					if(tempArray[0] == 0){
						tempArray[0] = 99;
					}
					else{
						tempArray[0]--;
					}

					tempArray[1] = 12;
					tempArray[settingFlag] = 31;
					return;
				}
				else{	// 其他
					tempArray[1]--;
					tempArray[settingFlag] = 31;
					return;
				}
			}

			if(SMALL_MONTH_DEC && (tempArray[settingFlag] == 1)){	// 小月
				tempArray[1]--;
				tempArray[settingFlag] = 30;
				return;
			}

			tempArray[settingFlag]--;	// 不符合前面的条件，项目值减1
			return;

		case 3:		// 小时合法判断
			if(tempArray[settingFlag] == 0)
			{
				tempArray[settingFlag] = 23;
				return;
			}
			tempArray[settingFlag]--;
			return;

		case 4:		// 分钟合法判断
			if(tempArray[settingFlag] == 0)
			{
				if(tempArray[3] == 0){
					tempArray[3] = 23;
				}
				else{
					tempArray[3]--;
				}
				tempArray[settingFlag] = 59;
				return;
			}
			tempArray[settingFlag]--;
			return;

		case 5:		// 秒钟合法判断
			if(tempArray[settingFlag] == 0)
			{
				if(tempArray[4] == 0){
					if(tempArray[3] == 0){
						tempArray[3] = 23;
						dealCrossingDay(DECREASE);
					}
					else{
						tempArray[3]--;
					}
					tempArray[4] = 59;
				}
				else{
					tempArray[4]--;
				}
				tempArray[settingFlag] = 59;
				return;
			}
			tempArray[settingFlag]--;
			return;

		default:
			return;
		}
}

// 处理设置时间跨越一天的情况
void dealCrossingDay(uint8_t incOrDes)
{
	if(incOrDes == INCREASE)	// 增加一天
	{
		if(LEAP_YEAR && FEBURARY && (tempArray[2] == 29)){	// 闰年二月
			tempArray[1]++;
			tempArray[2] = 1;
			return;
		}

		if(COMMON_YEAR && FEBURARY && (tempArray[2] == 28)){	// 平年二月
			tempArray[1]++;
			tempArray[2] = 1;
			return;
		}

		if(BIG_MONTH_INC && (tempArray[2] == 31)){	// 大月

			if((tempArray[1] == 12)){	// 十二月

				if(tempArray[0] == 99){
					tempArray[0] = 0;
				}
				else{
					tempArray[0]++;
				}

				tempArray[1] = 1;
				tempArray[2] = 1;
				return;
			}
			else{	// 其他
				tempArray[1]++;
				tempArray[2] = 1;
				return;
			}
		}

		if(SMALL_MONTH_INC && (tempArray[2] == 30)){	// 小月
			tempArray[1]++;
			tempArray[2] = 1;
			return;
		}

		tempArray[2]++;	// 不符合前面的条件，项目值加1
		return;
	}
	else	// 减少一天
	{
		if(LEAP_YEAR && MARCH && (tempArray[2] == 1)){	// 闰年三月
			tempArray[1]--;
			tempArray[2] = 29;
			return;
		}

		if(COMMON_YEAR && MARCH && (tempArray[2] == 1)){	// 平年三月
			tempArray[1]--;
			tempArray[2] = 28;
			return;
		}

		if(BIG_MONTH_DEC && (tempArray[2] == 1)){	// 大月后

			if((tempArray[1] == 1)){	// 十二月后

				if(tempArray[0] == 0){
					tempArray[0] = 99;
				}
				else{
					tempArray[0]--;
				}

				tempArray[1] = 12;
				tempArray[2] = 31;
				return;
			}
			else{	// 其他
				tempArray[1]--;
				tempArray[2] = 1;
				return;
			}
		}

		if(SMALL_MONTH_DEC && (tempArray[2] == 1)){	// 小月
			tempArray[1]--;
			tempArray[2] = 30;
			return;
		}

		tempArray[2]--;	// 不符合前面的条件，项目值减1
		return;
	}
}

// 显示当前日历
void RTC_TimeAndDate_Show(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);

	// 年月日
	OLED_ShowNumber(0,0,2,1,8);
	OLED_ShowNumber(10,0,0,1,8);
	if(RTC_DateStructure.Year > 9)
	{
		OLED_ShowNumber(20,0,RTC_DateStructure.Year,2,16);
	}
	else
	{
		OLED_ShowNumber(20,0,0,1,8);
		OLED_ShowNumber(30,0,RTC_DateStructure.Year,1,8);
	}

	oled_show_char(40, 0,'.', &fontone, SSD1306_COLOR_WHITE);

	if(RTC_DateStructure.Month > 9)
	{
		OLED_ShowNumber(50,0,RTC_DateStructure.Month,2,16);
	}
	else
	{
		OLED_ShowNumber(50,0,0,1,8);
		OLED_ShowNumber(60,0,RTC_DateStructure.Month,1,8);
	}

	oled_show_char(70, 0,'.', &fontone, SSD1306_COLOR_WHITE);

	if(RTC_DateStructure.Date > 9)
	{
		OLED_ShowNumber(80,0,RTC_DateStructure.Date,2,16);
	}
	else
	{
		OLED_ShowNumber(80,0,0,1,8);
		OLED_ShowNumber(90,0,RTC_DateStructure.Date,1,8);
	}

	// 时分秒
	if(flagAMPM ==1)
	{		// 开AM/PM模式
		if(RTC_TimeStructure.Hours >= 12)	// PM
			{
				oled_show_str(0,20,"PM", &fontone, SSD1306_COLOR_WHITE);
				if((RTC_TimeStructure.Hours - 12) > 9)
				{
					OLED_ShowNumber(20,20,(RTC_TimeStructure.Hours - 12),2,16);
				}
				else
				{
					OLED_ShowNumber(20,20,0,1,8);
					OLED_ShowNumber(30,20,(RTC_TimeStructure.Hours - 12),1,8);
				}
			}
			else	// AM
			{
				oled_show_str(0,20,"AM", &fontone, SSD1306_COLOR_WHITE);
				if(RTC_TimeStructure.Hours > 9)
				{
					OLED_ShowNumber(20,20,RTC_TimeStructure.Hours,2,16);
				}
				else
				{
					OLED_ShowNumber(20,20,0,1,8);
					OLED_ShowNumber(30,20,RTC_TimeStructure.Hours,1,8);
				}
			}
	}
	else
	{		// 关AM/PM模式
		if(RTC_TimeStructure.Hours > 9)
		{
			OLED_ShowNumber(20,20,RTC_TimeStructure.Hours,2,16);
		}
		else
		{
			OLED_ShowNumber(20,20,0,1,8);
			OLED_ShowNumber(30,20,RTC_TimeStructure.Hours,1,8);
		}
	}

	oled_show_char(40, 20,':', &fontone, SSD1306_COLOR_WHITE);

	if(RTC_TimeStructure.Minutes > 9)
	{
		OLED_ShowNumber(50,20,RTC_TimeStructure.Minutes,2,16);
	}
	else
	{
		OLED_ShowNumber(50,20,0,1,8);
		OLED_ShowNumber(60,20,RTC_TimeStructure.Minutes,1,8);
	}

	oled_show_char(70, 20,':', &fontone, SSD1306_COLOR_WHITE);

	if(RTC_TimeStructure.Seconds > 9)
	{
		OLED_ShowNumber(80,20,RTC_TimeStructure.Seconds,2,16);
	}
	else
	{
		OLED_ShowNumber(80,20,0,1,8);
		OLED_ShowNumber(90,20,RTC_TimeStructure.Seconds,1,8);
	}

	// 星期
	if(RTC_DateStructure.WeekDay == RTC_WEEKDAY_SUNDAY)
	{
		OLED_ShowNumber(0,40,7,1,8);
	}
	else
	{
		OLED_ShowNumber(0,40,RTC_DateStructure.WeekDay,1,8);
	}

	oled_update_screen();
}

// 显示节气
void showSolarTerms(void)
{
	loadSettingOrignalValue();

	switch(tempArray[1])
	{
	case 1:		// 一月
		if(tempArray[2] <= 5){     //1月5日小寒，1月20日大寒
			oled_show_china(0,0,12, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,25, SSD1306_COLOR_WHITE);        //小寒
		}

		else if(tempArray[2] <= 20){
			oled_show_china(0,0,18, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,25, SSD1306_COLOR_WHITE);       //大寒
		}
		else{
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,6, SSD1306_COLOR_WHITE);        //立春
		}
		break;

	case 2:    //二月
		if(tempArray[2] <= 4){     //2月4日立春，2月18日雨水
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,6, SSD1306_COLOR_WHITE);        //立春

		}

		else if(tempArray[2]<=18){
			oled_show_china(0,0,2, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,3, SSD1306_COLOR_WHITE);       //雨水
		}

		else{
			oled_show_china(0,0,4, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,5, SSD1306_COLOR_WHITE);        //惊蛰
		}
		break;

	case 3:   //三月
		if(tempArray[2] <= 6){     //3月6日惊蛰，3月21日春分
			oled_show_china(0,0,4, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,5, SSD1306_COLOR_WHITE);        //惊蛰
		}

		else if(tempArray[2]<=21){
			oled_show_china(0,0,6, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,7, SSD1306_COLOR_WHITE);       //春分
		}

		else{
			oled_show_china(0,0,8, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,9, SSD1306_COLOR_WHITE);        //清明

		}
		break;

	case 4:     //四月
		if(tempArray[2]<=4){     //4月4日清明，4月19日谷雨
			oled_show_china(0,0,8, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,9, SSD1306_COLOR_WHITE);        //清明
		}

		else if(tempArray[2]<=19){
			oled_show_china(0,0,10, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,2, SSD1306_COLOR_WHITE);       //谷雨
		}
		else{
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,16, SSD1306_COLOR_WHITE);        //立夏
		}
		break;

	case 5:      //五月
		if(tempArray[2]<=5){     //5月5日立夏，5月20日小满
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,16, SSD1306_COLOR_WHITE);        //立夏

		}

		else if(tempArray[2]<=20){
			oled_show_china(0,0,12, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,13, SSD1306_COLOR_WHITE);       //小满

		}

		else{
			oled_show_china(0,0,14, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,15, SSD1306_COLOR_WHITE);        //芒种
		}
		break;

	case 6:     //六月
		if(tempArray[2]<=5){     //6月5日芒种，6月21日夏至
			oled_show_china(0,0,14, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,15, SSD1306_COLOR_WHITE);        //芒种
		}

		else if(tempArray[2]<=21){
			oled_show_china(0,0,16, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,17, SSD1306_COLOR_WHITE);       //夏至

		}
		else{
			oled_show_china(0,0,12, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,19, SSD1306_COLOR_WHITE);        //小暑
		}
		break;

	case 7:       //七月
		if(tempArray[2]<=6){     //7月6日小暑，7月22日大暑
			oled_show_china(0,0,12, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,19, SSD1306_COLOR_WHITE);        //小暑
		}

		else if(tempArray[2]<=22){
			oled_show_china(0,0,18, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,19, SSD1306_COLOR_WHITE);       //大暑
		}
		else{
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,24, SSD1306_COLOR_WHITE);        //立秋
		}
		break;

	case 8:        //八月
		if(tempArray[2]<=7){     //8月7日立秋，8月22日处暑
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,24, SSD1306_COLOR_WHITE);        //立秋

		}

		else if(tempArray[2]<=22){
			oled_show_china(0,0,21, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,19, SSD1306_COLOR_WHITE);       //处暑

		}
		else{
			oled_show_china(0,0,22, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,23, SSD1306_COLOR_WHITE);        //白露
		}
		break;

	case 9:       //九月
		if(tempArray[2]<=7){     //9月7日白露，9月22日秋分
			oled_show_china(0,0,22, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,23, SSD1306_COLOR_WHITE);        //白露
		}

		else if(tempArray[2]<=22){
			oled_show_china(0,0,24, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,7, SSD1306_COLOR_WHITE);       //秋分

		}
		else{
			oled_show_china(0,0,25, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,23, SSD1306_COLOR_WHITE);        //寒露
		}
		break;

	case 10:      //十月
		if(tempArray[2]<=7){     //10月7日寒露，10月23日霜降
			oled_show_china(0,0,25, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,23, SSD1306_COLOR_WHITE);        //寒露
		}

		else if(tempArray[2]<=23){
			oled_show_china(0,0,26, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,27, SSD1306_COLOR_WHITE);       //霜降
		}

		else{
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,30, SSD1306_COLOR_WHITE);        //立冬
		}
		break;

	case 11:       //十一月
		if(tempArray[2]<=7){     //11月7日立冬，11月22日小雪
			oled_show_china(0,0,0, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,30, SSD1306_COLOR_WHITE);        //立冬

		}

		else if(tempArray[2]<=22){
			oled_show_china(0,0,12, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,29, SSD1306_COLOR_WHITE);       //小雪

		}

		else{
			oled_show_china(0,0,18, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,29, SSD1306_COLOR_WHITE);        //大雪
		}
		break;

	case 12:        //十二月
		if(tempArray[2]<=6){     //12月6日大雪，12月21日冬至
			oled_show_china(0,0,18, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,29, SSD1306_COLOR_WHITE);        //大雪

		}

		else if(tempArray[2]<=21){
			oled_show_china(0,0,30, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,17, SSD1306_COLOR_WHITE);       //冬至

		}

		else{
			oled_show_china(0,0,12, SSD1306_COLOR_WHITE);
			oled_show_china(20,0,25, SSD1306_COLOR_WHITE);        //小寒
		}
		break;
	}

	oled_update_screen();
}

// 切屏2：节气部分
void showSolarTermsScreen2(void)
{
	loadSettingOrignalValue();

	switch(tempArray[1])
	{
	case 1:		// 一月
		if(tempArray[2] <= 5){     //1月5日小寒，1月20日大寒
			oled_show_china(20,20,12, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,25, SSD1306_COLOR_WHITE);        //小寒
		}

		else if(tempArray[2] <= 20){
			oled_show_china(20,20,18, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,25, SSD1306_COLOR_WHITE);       //大寒
		}
		else{
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,6, SSD1306_COLOR_WHITE);        //立春
		}
		break;

	case 2:    //二月
		if(tempArray[2] <= 4){     //2月4日立春，2月18日雨水
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,6, SSD1306_COLOR_WHITE);        //立春

		}

		else if(tempArray[2]<=18){
			oled_show_china(20,20,2, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,3, SSD1306_COLOR_WHITE);       //雨水
		}

		else{
			oled_show_china(20,20,4, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,5, SSD1306_COLOR_WHITE);        //惊蛰
		}
		break;

	case 3:   //三月
		if(tempArray[2] <= 6){     //3月6日惊蛰，3月21日春分
			oled_show_china(20,20,4, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,5, SSD1306_COLOR_WHITE);        //惊蛰
		}

		else if(tempArray[2]<=21){
			oled_show_china(20,20,6, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,7, SSD1306_COLOR_WHITE);       //春分
		}

		else{
			oled_show_china(20,20,8, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,9, SSD1306_COLOR_WHITE);        //清明

		}
		break;

	case 4:     //四月
		if(tempArray[2]<=4){     //4月4日清明，4月19日谷雨
			oled_show_china(20,20,8, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,9, SSD1306_COLOR_WHITE);        //清明
		}

		else if(tempArray[2]<=19){
			oled_show_china(20,20,10, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,2, SSD1306_COLOR_WHITE);       //谷雨
		}
		else{
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,16, SSD1306_COLOR_WHITE);        //立夏
		}
		break;

	case 5:      //五月
		if(tempArray[2]<=5){     //5月5日立夏，5月20日小满
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,16, SSD1306_COLOR_WHITE);        //立夏

		}

		else if(tempArray[2]<=20){
			oled_show_china(20,20,12, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,13, SSD1306_COLOR_WHITE);       //小满

		}

		else{
			oled_show_china(20,20,14, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,15, SSD1306_COLOR_WHITE);        //芒种
		}
		break;

	case 6:     //六月
		if(tempArray[2]<=5){     //6月5日芒种，6月21日夏至
			oled_show_china(20,20,14, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,15, SSD1306_COLOR_WHITE);        //芒种
		}

		else if(tempArray[2]<=21){
			oled_show_china(20,20,16, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,17, SSD1306_COLOR_WHITE);       //夏至

		}
		else{
			oled_show_china(20,20,12, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,19, SSD1306_COLOR_WHITE);        //小暑
		}
		break;

	case 7:       //七月
		if(tempArray[2]<=6){     //7月6日小暑，7月22日大暑
			oled_show_china(20,20,12, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,19, SSD1306_COLOR_WHITE);        //小暑
		}

		else if(tempArray[2]<=22){
			oled_show_china(20,20,18, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,19, SSD1306_COLOR_WHITE);       //大暑
		}
		else{
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,24, SSD1306_COLOR_WHITE);        //立秋
		}
		break;

	case 8:        //八月
		if(tempArray[2]<=7){     //8月7日立秋，8月22日处暑
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,24, SSD1306_COLOR_WHITE);        //立秋

		}

		else if(tempArray[2]<=22){
			oled_show_china(20,20,21, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,19, SSD1306_COLOR_WHITE);       //处暑

		}
		else{
			oled_show_china(20,20,22, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,23, SSD1306_COLOR_WHITE);        //白露
		}
		break;

	case 9:       //九月
		if(tempArray[2]<=7){     //9月7日白露，9月22日秋分
			oled_show_china(20,20,22, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,23, SSD1306_COLOR_WHITE);        //白露
		}

		else if(tempArray[2]<=22){
			oled_show_china(20,20,24, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,7, SSD1306_COLOR_WHITE);       //秋分

		}
		else{
			oled_show_china(20,20,25, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,23, SSD1306_COLOR_WHITE);        //寒露
		}
		break;

	case 10:      //十月
		if(tempArray[2]<=7){     //10月7日寒露，10月23日霜降
			oled_show_china(20,20,25, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,23, SSD1306_COLOR_WHITE);        //寒露
		}

		else if(tempArray[2]<=23){
			oled_show_china(20,20,26, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,27, SSD1306_COLOR_WHITE);       //霜降
		}

		else{
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,30, SSD1306_COLOR_WHITE);        //立冬
		}
		break;

	case 11:       //十一月
		if(tempArray[2]<=7){     //11月7日立冬，11月22日小雪
			oled_show_china(20,20,0, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,30, SSD1306_COLOR_WHITE);        //立冬

		}

		else if(tempArray[2]<=22){
			oled_show_china(20,20,12, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,29, SSD1306_COLOR_WHITE);       //小雪

		}

		else{
			oled_show_china(20,20,18, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,29, SSD1306_COLOR_WHITE);        //大雪
		}
		break;

	case 12:        //十二月
		if(tempArray[2]<=6){     //12月6日大雪，12月21日冬至
			oled_show_china(20,20,18, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,29, SSD1306_COLOR_WHITE);        //大雪

		}

		else if(tempArray[2]<=21){
			oled_show_china(20,20,30, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,17, SSD1306_COLOR_WHITE);       //冬至

		}

		else{
			oled_show_china(20,20,12, SSD1306_COLOR_WHITE);
			oled_show_china(40,20,25, SSD1306_COLOR_WHITE);        //小寒
		}
		break;
	}
}
