/*
 * dht11.c
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#include "dht11.h"
#include "delayUs.h"
#include "oled.h"

uint8_t data[4];//接收数据

//数据输入输出为PA8
void Data_Out_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Structure.Pin = DHT11_Pin;
	GPIO_Structure.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_Structure);
}

void Data_In_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Structure.Mode = GPIO_MODE_INPUT;
	GPIO_Structure.Pull = GPIO_NOPULL;
	GPIO_Structure.Pin = DHT11_Pin;
	GPIO_Structure.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_Structure);
}


/*
	首先主机发送开始信号，即：先拉高数据线
	再拉低数据线，保持 t1（至少 18ms）时间
	然后拉高数据线 t2（20~40us）时间，然后读取 DHT11 的响应

	正常的话，DHT11 会拉低数据线，保持 t3（40~50us）时间，作为响应信号
	然后 DHT11 拉高数据线，保持 t4（40~50us）时间后，开始输出数据。
*/

void DHT11_start(void)
{
	Data_Out_Init();
	HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET);
	delayUs(5);
	HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET);
	HAL_Delay(20);   //拉低18ms以上  20ms
	HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET);
	delayUs(30);   // 拉高20us-40us
	Data_In_Init();
	delayUs(10);
}

uint8_t DHT11_rec_byte(void)      //接收一个字节
{
	uint8_t i,data=0;
	for(i=0;i<8;i++)    //从高到低依次接收8位数据
	{
		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 0);   //等待50us低电平过去
		delayUs(40);     //延时40us，如果还为高则数据为1，否则为0
		data<<=1;          //移位使正确接收8位数据，数据为0时直接移位
		if(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 1)    //数据为1时，使dat加1来接收数据1
		{
			data+=1;
		}
		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 1);  //等待数据线拉低
	}
	return data;
}

void DHT11_rec_data(uint8_t *tempH,uint8_t *tempL,uint8_t *decH,uint8_t *decL)      //接收40位的数据
{
	uint8_t R_H,R_L,T_H,T_L,revise;
	DHT11_start();
	if(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 0)
	{

		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 0);//处理响应低电平，等待拉高

		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 1);//处理响应高电平，等待拉低

		R_H=DHT11_rec_byte();    //接收湿度高八位
		R_L=DHT11_rec_byte();    //接收湿度低八位
		T_H=DHT11_rec_byte();    //接收温度高八位
		T_L=DHT11_rec_byte();    //接收温度低八位
		revise=DHT11_rec_byte(); //接收校正位

		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin) == 0);//等待从机拉低

		if((R_H+R_L+T_H+T_L) == revise)    //校正
		{
			data[0]=T_H; //温度高8位(表示整数部分)
			data[1]=T_L; //温度低8位(表示小数部分)
			data[2]=R_H; //湿度高8位(表示整数部分)
			data[3]=R_L; //湿度低8位(表示小数部分)

			*tempH=data[0];//温度
			*tempL=data[1];
			*decH=data[2];//湿度
			*decL=data[3];
		}

  }
}

//显示温湿度
void showTempAndHumi(void)
{
	uint8_t receive_tempH, receive_tempL,receive_humH,receive_humL;
	uint32_t DHT11_data[4]={0};

	DHT11_rec_data(&receive_tempH,&receive_tempL ,&receive_humH,&receive_humL);
	DHT11_data[0]=receive_tempH;
	DHT11_data[1]=receive_tempL;
	DHT11_data[2]=receive_humH;
	DHT11_data[3]=receive_humL;

	oled_show_china(0,0,35, SSD1306_COLOR_WHITE);
	oled_show_china(20,0,36, SSD1306_COLOR_WHITE);   //温度
	OLED_ShowNumber(40,0,DHT11_data[0],2,16);		//温度整数部分
	oled_show_char(58,0,'.', &fontone,SSD1306_COLOR_WHITE);
	OLED_ShowNumber(68,0,DHT11_data[1],2,16);		//温度小数部分
	oled_show_str(86,0,"'C", &fontone,SSD1306_COLOR_WHITE);

	oled_show_china(0,20,37, SSD1306_COLOR_WHITE);
	oled_show_china(20,20,36, SSD1306_COLOR_WHITE);   //湿度
	OLED_ShowNumber(40,20,DHT11_data[2],2,16);		//湿度整数部分
	oled_show_char(58,20,'.', &fontone,SSD1306_COLOR_WHITE);
	OLED_ShowNumber(68,20,DHT11_data[3],2,16);		//湿度小数部分
	oled_show_str(86,20,"%RH", &fontone,SSD1306_COLOR_WHITE);

	oled_update_screen();
}
