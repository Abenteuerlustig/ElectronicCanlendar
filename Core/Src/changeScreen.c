/*
 * changeScreen.c
 *
 *  Created on: 2023年7月1日
 *      Author: dell
 */

#include "changeScreen.h"

void screen1(void)
{
	// 温度
	oled_show_china(0,0,35, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,36, SSD1306_COLOR_WHITE);
	oled_show_str(32, 0, ":", &fontone, SSD1306_COLOR_WHITE);
	OLED_ShowNumber(40,0,flagTemp,2,16);

	// 湿度
	oled_show_china(0,20,37, SSD1306_COLOR_WHITE);
	oled_show_china(16,20,36, SSD1306_COLOR_WHITE);
	oled_show_str(32, 20, ":", &fontone, SSD1306_COLOR_WHITE);
	OLED_ShowNumber(40,20,flagHumi,2,16);

	// 天气
	showWeather();

	oled_update_screen();
}

void screen2(void)
{
	if(flagRun == 1)
	{
		oled_show_china(0,0,77, SSD1306_COLOR_WHITE);
	}

	showMonth();
	showDay();
	showSolarTermsScreen2();

	oled_update_screen();
}
