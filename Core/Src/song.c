/*
 * song.c
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#include "song.h"

// 停止播放音乐
void stopMusic(void)
{
	uint8_t stopArray[]={0x7E,0xFF,0x06,0x16,0x00,0x00,0x00,0xFE,0xE5,0xEF};
	HAL_UART_Transmit(&huart3,(uint8_t *)stopArray,sizeof(stopArray),0xFFFF);
}

// 播放音乐（有参、闹钟）
void playMusicWithParameter(uint8_t selectMusicId)
{
	uint8_t mp1[]={0x7E,0xEF,0x06,0x02,0x00,0x00,0x00,0xEF};		//上一首
	uint8_t mp2[]={0x7E,0xFF,0x06,0x01,0x00,0x00,0x00,0xEF};		//下一首
	uint8_t mp3[]={0x7E,0xFF,0x06,0x16,0x00,0x00,0x00,0xEF};		//暂停


	switch(selectMusicId)
	{
	case 1:
		if(musicId <= 1) musicId = 10;
		else musicId--;
		HAL_UART_Transmit(&huart3,(uint8_t *)mp1,sizeof(mp1),0xFFFF);
		break;

	case 2:
		if(musicId >= 10) musicId = 1;
		else musicId++;
		HAL_UART_Transmit(&huart3,(uint8_t *)mp2,sizeof(mp2),0xFFFF);
		break;

	case 3:
		HAL_UART_Transmit(&huart3,(uint8_t *)mp3,sizeof(mp3),0xFFFF);
		break;

	default:
		break;
	}
}

// 提示进入选择（闹钟）
void tipsEnter(void)
{
	oled_show_china(8,0,68, SSD1306_COLOR_WHITE);
	oled_show_str(24,0,"KEY4",&fontone,SSD1306_COLOR_WHITE);
	oled_show_china(56,0,69, SSD1306_COLOR_WHITE);
	oled_show_china(72,0,70, SSD1306_COLOR_WHITE);
	oled_show_china(88,0,33, SSD1306_COLOR_WHITE);
	oled_show_china(102,0,34, SSD1306_COLOR_WHITE);
	oled_update_screen();
}

// 提示选择音乐中（闹钟）
void tipsSelect(void)
{
	oled_show_china(0,0,69, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,70, SSD1306_COLOR_WHITE);
	oled_show_china(32,0,33, SSD1306_COLOR_WHITE);
	oled_show_china(48,0,34, SSD1306_COLOR_WHITE);
	oled_show_china(64,0,38, SSD1306_COLOR_WHITE);
	oled_show_str(80,0,"...",&fontone,SSD1306_COLOR_WHITE);
	oled_show_china(0,20,68, SSD1306_COLOR_WHITE);
	oled_show_str(16,20,"KEY4",&fontone,SSD1306_COLOR_WHITE);
	oled_show_china(48,20,62, SSD1306_COLOR_WHITE);
	oled_show_china(64,20,63, SSD1306_COLOR_WHITE);
	oled_show_china(80,20,69, SSD1306_COLOR_WHITE);
	oled_show_china(96,20,70, SSD1306_COLOR_WHITE);
	oled_update_screen();
}

// 选取音乐（闹钟）
void selectSong(void)
{
	int keyValueInAlarmClock= readKeyValue();
	uint8_t commandFromBluetooth = returnFlagBluetooth();
	clearBluetoothCommand();
	oled_clear();

	//重置音乐
	resetMusic();

	// 没有按下选择音乐键时，循环提示
	while((keyValueInAlarmClock != 4) && (commandFromBluetooth != 11))
	{
		// 提示：按KEY4选择音乐
		tipsEnter();
		// 再次读取按键值、再次读取蓝牙命令
		keyValueInAlarmClock= readKeyValue();
		commandFromBluetooth = returnFlagBluetooth();
		clearBluetoothCommand();
	}

	// 再次读取按键值、再次读取蓝牙命令
	keyValueInAlarmClock = readKeyValue();
	commandFromBluetooth = returnFlagBluetooth();
	clearBluetoothCommand();
	oled_clear();

	// 选择键按下，确认键没有按下时
	while((keyValueInAlarmClock != 4) && (commandFromBluetooth != 22))
	{
		// 提示：选择音乐中
		tipsSelect();

		// 试听音乐
		if((keyValueInAlarmClock == 1) || (keyValueInAlarmClock == 2) || (keyValueInAlarmClock == 3))
			playMusicWithParameter(keyValueInAlarmClock);
		if((commandFromBluetooth == 19) || (commandFromBluetooth == 20) || (commandFromBluetooth == 21))
			playMusicWithParameter(commandFromBluetooth - 18);

		// 再次读取按键值、再次读取蓝牙命令
		keyValueInAlarmClock= readKeyValue();
		commandFromBluetooth = returnFlagBluetooth();
		clearBluetoothCommand();
	}

	// 停止播放音乐，选择完成
	stopMusic();
}

// 选取MP3音乐
void selectMusicMP3(void)
{
	int keyValueInAlarmClock= readKeyValue();
	uint8_t commandFromBluetooth = returnFlagBluetooth();
	clearBluetoothCommand();

	//重置音乐
	resetMusic();

	// 选择键按下，确认键没有按下时
	while((keyValueInAlarmClock != 4) && (commandFromBluetooth != 22))
	{
		// 提示：选择音乐中
		tipMP3();


		// 试听音乐
		// 加一个是否播放的校验位：未完成
		if((keyValueInAlarmClock == 1) || (keyValueInAlarmClock == 2) || (keyValueInAlarmClock == 3))
			playMusicMP3(keyValueInAlarmClock);
		if((commandFromBluetooth == 19) || (commandFromBluetooth == 20) || (commandFromBluetooth == 21))
			playMusicMP3(commandFromBluetooth - 18);

		// 再次读取按键值、再次读取蓝牙命令
		keyValueInAlarmClock= readKeyValue();
		commandFromBluetooth = returnFlagBluetooth();
		clearBluetoothCommand();
	}

	// 停止播放音乐，选择完成
	stopMusic();
}

// 播放音乐（MP3）
void playMusicMP3(uint8_t selectMusicId)
{
	uint8_t mp1[]={0x7E,0xEF,0x06,0x02,0x00,0x00,0x00,0xEF};		//上一首
	uint8_t mp2[]={0x7E,0xFF,0x06,0x01,0x00,0x00,0x00,0xEF};		//下一首
	uint8_t mp3[]={0x7E,0xFF,0x06,0x16,0x00,0x00,0x00,0xEF};		//暂停

	switch(selectMusicId)
	{
	case 1:
		HAL_UART_Transmit(&huart3,(uint8_t *)mp1,sizeof(mp1),0xFFFF);
		break;

	case 2:
		HAL_UART_Transmit(&huart3,(uint8_t *)mp2,sizeof(mp2),0xFFFF);
		break;

	case 3:
		HAL_UART_Transmit(&huart3,(uint8_t *)mp3,sizeof(mp3),0xFFFF);
		break;

	default:
		break;
	}
}

// 提示：MP3播放音乐中
void tipMP3(void)
{
	oled_show_str(0,0,"MP3",&fontone,SSD1306_COLOR_WHITE);
	oled_show_china(24,0,33, SSD1306_COLOR_WHITE);
	oled_show_china(40,0,34, SSD1306_COLOR_WHITE);
	oled_show_str(56,0,"...",&fontone,SSD1306_COLOR_WHITE);
	oled_show_str(0,20,"EXIT:KEY4",&fontone,SSD1306_COLOR_WHITE);
	oled_update_screen();
}

// 闹钟播放
void alarmClockMusic(void)
{
	uint8_t music1[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x01,0xEF};
	uint8_t music2[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x02,0xEF};
	uint8_t music3[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x03,0xEF};
	uint8_t music4[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x04,0xEF};
	uint8_t music5[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x05,0xEF};
	uint8_t music6[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x06,0xEF};
	uint8_t music7[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x07,0xEF};
	uint8_t music8[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x08,0xEF};
	uint8_t music9[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x09,0xEF};
	uint8_t music10[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x10,0xEF};
	switch(musicId)
	{
	case 1:
		HAL_UART_Transmit(&huart3,(uint8_t *)music1,sizeof(music1),0xFFFF);
		break;

	case 2:
		HAL_UART_Transmit(&huart3,(uint8_t *)music2,sizeof(music2),0xFFFF);
		break;

	case 3:
		HAL_UART_Transmit(&huart3,(uint8_t *)music3,sizeof(music3),0xFFFF);
		break;

	case 4:
		HAL_UART_Transmit(&huart3,(uint8_t *)music4,sizeof(music4),0xFFFF);
		break;

	case 5:
		HAL_UART_Transmit(&huart3,(uint8_t *)music5,sizeof(music5),0xFFFF);
		break;

	case 6:
		HAL_UART_Transmit(&huart3,(uint8_t *)music6,sizeof(music6),0xFFFF);
		break;

	case 7:
		HAL_UART_Transmit(&huart3,(uint8_t *)music7,sizeof(music7),0xFFFF);
		break;

	case 8:
		HAL_UART_Transmit(&huart3,(uint8_t *)music8,sizeof(music8),0xFFFF);
		break;

	case 9:
		HAL_UART_Transmit(&huart3,(uint8_t *)music9,sizeof(music9),0xFFFF);
		break;

	case 10:
		HAL_UART_Transmit(&huart3,(uint8_t *)music10,sizeof(music10),0xFFFF);
		break;

	default:
		break;

	}
}

//重置音乐
void resetMusic(void)
{
	musicId = 1;
	uint8_t reset[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x01,0xEF};
	HAL_UART_Transmit(&huart3,(uint8_t *)reset,sizeof(reset),0xFFFF);
	HAL_Delay(100);
	stopMusic();
}
