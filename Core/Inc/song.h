/*
 * song.h
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#ifndef INC_SONG_H_
#define INC_SONG_H_

#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "oled.h"
#include "myVolatile.h"
#include <stdint.h>

void stopMusic(void);
void playMusicWithParameter(uint8_t keyValue);
void tipsEnter(void);
void tipsSelect(void);
void selectSong(void);
void selectMusicMP3(void);
void playMusicMP3(uint8_t selectMusicId);
void tipMP3(void);
void resetMusic(void);
void alarmClockMusic(void);

#endif /* INC_SONG_H_ */
