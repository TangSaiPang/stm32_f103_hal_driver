#include "bsp_tb6642.h"

void TB6642_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

	/*Configure GPIO pins : PAPin */
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*
 * @brief: Motor break stop
 * @param: None
 * @retval None
 * */
void TB6642_BreakToStop(void)
{
	HAL_GPIO_WritePin(GPIOA, TB6642_IN1_PIN|TB6642_IN2_PIN, GPIO_PIN_SET);
}

/*
 * @brief: Motor output in high resistance
 * @param: None
 * @retval None
 * */
void TB6642_High_Resistance(void)
{
	HAL_GPIO_WritePin(GPIOA, TB6642_IN1_PIN|TB6642_IN2_PIN, GPIO_PIN_RESET);
}

/*
 * @brief: Set motor speed
 * @param: Duty
 * @retval None
 * */
void TB6642_SetSpeed(uint16_t duty)
{
	if (duty > 2000)
	{
		return;
	}
	uint16_t ccr = duty * 2000 / 2000;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
}

/*
 * @brief: Set motor direction
 * @param: Dir MOTOR_DIR_CCW ,MOTOR_DIR_CW
 * @retval None
 * */
void TB6642_SetMotorDir(MotorDir_Typedef dir)
{
	if (dir == MOTOR_DIR_CCW)
	{
		HAL_GPIO_WritePin(GPIOA, TB6642_IN1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, TB6642_IN2_PIN, GPIO_PIN_RESET);
	}

	if (dir == MOTOR_DIR_CW)
	{
		HAL_GPIO_WritePin(GPIOA, TB6642_IN1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, TB6642_IN2_PIN, GPIO_PIN_SET);
	}	
}

/*
 * @brief: Set motor enable or disable
 * @param: Ena MOTOR_ENABLE or MOTOR_DISABLE 
 * @retval None
 * */
void TB6642_SetMotorEna(MotorSta_Typedef ena)
{
	if (ena == MOTOR_ENABLE)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}
	if (ena == MOTOR_DISABLE)
	{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
}


