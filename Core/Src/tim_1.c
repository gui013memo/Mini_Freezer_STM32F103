/*
 * tim_1.c
 *
 *  Created on: Feb 6, 2022
 *      Author: GUI013
 */

#include "main.h"

extern TIM_HandleTypeDef htim1;

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}
