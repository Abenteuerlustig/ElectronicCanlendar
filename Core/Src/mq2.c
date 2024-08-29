/*
 * mq2.c
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#include "mq2.h"
#include "led.h"
#include "song.h"
#include "oled.h"
#include "usart.h"

/***************************************************************************
 * 描  述 : 气体传感器检测函数（检测开关量信号）
 * 参  数 : 无
 * 返回值 : 无
 **************************************************************************/
void MQ2_Scan(void)
{
     // 调用库函数GPIO_ReadInputDataBit()检测气体传感器开关量检测引脚的电平是否为低电平
     if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET)
     {
    	 // 软件延时10ms，防信号干扰
    	 HAL_Delay(10);

    	 if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET )
    	 {
        	LED_SET_ON;		// 点亮LED
        	oled_clear();		// 清屏
        	if(flagSendMessageForFireAlarm == 0)
        	{
        		HAL_UART_Transmit(&huart2, (uint8_t *)1, 1, 1000);		// 向串口2发送1字节数据
        		flagSendMessageForFireAlarm = 1;
        	}
			fireAlarm();		// 警报声
			displayFirmAlarm();		// 报警显示

        	//等待检测信号释放，即如果PB5一直为低电平，会一直执行空命令
        	while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET)
        	{
        		LED_Toggle();		// LED闪烁
        	}

        	//检测信号释放,熄灭LED,关闭警报
        	LED_SET_OFF;
        	oled_clear();
        	stopMusic();
        	flagSendMessageForFireAlarm = 0;
        }
     }
     else
     {
    	 LED_SET_OFF;
    	 flagSendMessageForFireAlarm = 0;
     }
}

// 报警声
void fireAlarm(void)
{
	uint8_t fireAlarm[]={0x7E,0xFF,0x06,0x03,0x00,0x00,0x08,0xEF};
	HAL_UART_Transmit(&huart3,(uint8_t *)fireAlarm,sizeof(fireAlarm),0xFFFF);
}

// 报警显示
void displayFirmAlarm(void)
{
	oled_show_china(0,0,20, SSD1306_COLOR_WHITE);
	oled_show_china(16,0,11, SSD1306_COLOR_WHITE);
	oled_show_str(32,0,"!!!",&fontone,SSD1306_COLOR_WHITE);
	oled_update_screen();
}
