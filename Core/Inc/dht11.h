/*
 * dht11.h
 *
 *  Created on: 2023年6月28日
 *      Author: dell
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "main.h"

void Data_Out_Init(void);
void Data_In_Init(void);
void DHT11_start(void);
uint8_t DHT11_rec_byte(void);
void DHT11_rec_data(uint8_t *tempH,uint8_t *tempL,uint8_t *decH,uint8_t *decL);
void showTempAndHumi(void);

#endif /* INC_DHT11_H_ */
