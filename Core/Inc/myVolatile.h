/*
 * myVolatile.h
 *
 *  Created on: 2023年6月27日
 *      Author: dell
 */

#ifndef INC_MYVOLATILE_H_
#define INC_MYVOLATILE_H_

#include "main.h"

extern volatile uint8_t tempArray[7];		// 设置时间日期设定值
extern volatile uint8_t tempArrayAlarmClock[6];		// 闹钟设定值
extern volatile int musicId;		// 音乐ID
extern volatile uint8_t flagAMPM;		// AM/PM标志
extern volatile uint8_t flagBluetooth;		// 蓝牙命令标志
extern volatile uint8_t flagWeather;
extern volatile uint8_t flagTemp;
extern volatile uint8_t flagHumi;
extern volatile uint8_t flagRun;
extern volatile uint8_t flagMonth;
extern volatile uint8_t flagDay;
extern volatile uint8_t flagSendMessageForFireAlarm;

void loadSettingOrignalValue(void);
uint8_t returnFlagBluetooth(void);
void clearBluetoothCommand(void);
void showWeather();
void showMonth();
void showDay();

#define LEAP_YEAR (tempArray[0] % 4 == 0)
#define COMMON_YEAR (tempArray[0] % 4 != 0)
#define BIG_MONTH_INC ((tempArray[1] == 1) || (tempArray[1] == 3) || (tempArray[1] == 5) || (tempArray[1] == 7) || (tempArray[1] == 8) || (tempArray[1] == 10) || (tempArray[1] == 12))
#define SMALL_MONTH_INC ((tempArray[1] == 4) || (tempArray[1] == 6) || (tempArray[1] == 9) || (tempArray[1] == 11))
#define BIG_MONTH_DEC ((tempArray[1] == 2) || (tempArray[1] == 4) || (tempArray[1] == 6) || (tempArray[1] == 8) || (tempArray[1] == 9) || (tempArray[1] == 11) || (tempArray[1] == 1))
#define SMALL_MONTH_DEC ((tempArray[1] == 5) || (tempArray[1] == 7) || (tempArray[1] == 10) || (tempArray[1] == 12))
#define FEBURARY (tempArray[1] == 2)
#define MARCH (tempArray[1] == 3)
#define INCREASE 1
#define DECREASE 0

#endif /* INC_MYVOLATILE_H_ */
