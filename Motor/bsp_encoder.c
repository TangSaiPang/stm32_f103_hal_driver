/*
 * bsp_encoder.c
 *
 *  Created on: Jun 30, 2024
 *      Author: Saipa
 */

#include "bsp_encoder.h"

int32_t ENCODER_Get_Value(void)
{
	uint32_t temp = __HAL_TIM_GET_COUNTER(&htim2);
//	__HAL_TIM_SET_COUNTER(&htim2, 0);
	return temp;
}

float Encoder_Get_Motor_Speed(void)
{
	int32_t temp = 0;
	temp = ENCODER_Get_Value();
	return (float)temp/(float)ENCODER_RESOLUTION;
}

