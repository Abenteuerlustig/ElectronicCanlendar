/*
 * alarmClock.h
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#ifndef INC_ALARMCLOCK_H_
#define INC_ALARMCLOCK_H_

#include "show.h"
#include "song.h"

void extranceAlarmClock(void);
void setDateAndTimeShowAlarmClock(int settingFlag);
void alarmClockReminder(void);
void checkAlarmClock(void);

#endif /* INC_ALARMCLOCK_H_ */
