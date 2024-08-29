/*
 * led.h
 *
 *  Created on: 2023年6月26日
 *      Author: dell
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

#define LED_SET_ON (HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET))
#define LED_SET_OFF (HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET))
#define LED_ON (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin) == GPIO_PIN_SET)
#define LED_OFF (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin) == GPIO_PIN_RESET)

void LED_Toggle(void);

#endif /* INC_LED_H_ */
