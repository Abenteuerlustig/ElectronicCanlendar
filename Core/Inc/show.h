/*
 * show.h
 *
 *  Created on: 2023年6月26日
 *      Author: dell
 */

#ifndef INC_SHOW_H_
#define INC_SHOW_H_

#include "myVolatile.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include <stdint.h>
#include "rtc.h"
#include "usart.h"

void extranceDateAndTime(void);
void setDateAndTimeShow(int settingFlag);		// 看情况用，目前还没用到参数
void Increase(int settingFlag);
void diaplaySettingAMPM(void);
void settingAMPM(void);
void Decrease(int settingFlag);
void dealCrossingDay(uint8_t incOrDes);
void RTC_TimeAndDate_Show(void);
void showSolarTerms(void);
void showSolarTermsScreen2(void);

#endif /* INC_SHOW_H_ */
