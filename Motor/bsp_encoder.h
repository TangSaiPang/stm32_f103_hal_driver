/*
 * bsp_encoder.h
 *
 *  Created on: Jun 30, 2024
 *      Author: Saipa
 */

#ifndef INC_BSP_ENCODER_H_
#define INC_BSP_ENCODER_H_

#include "main.h"
#include "tim.h"

typedef struct
{
	int32_t Value_Old;
	int32_t Value_Now;
	int32_t Encode_Speed;
} Encoder_Typdef;

#define ENCODER_RESOLUTION_SIMPLEX	100
#define ENCODER_REDUCTION_RATIO		270
#define ENCODER_RESOLUTION			(ENCODER_RESOLUTION_SIMPLEX * ENCODER_REDUCTION_RATIO * 4)

int32_t ENCODER_Get_Value(void);
float Encoder_Get_Motor_Speed(void);

#endif /* INC_BSP_ENCODER_H_ */
