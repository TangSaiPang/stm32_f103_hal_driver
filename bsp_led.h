#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f1xx.h"

#define USE_REG 0

#if USE_REG == 0
// LED0: PA8	LED2: PD2
#define LED0_Pin GPIO_PIN_8
#define LED0_GPIO_Port GPIOA
#define LED0_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOD
#define LED1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#else
#define LED0_ON()		WRITE_REG(GPIOA->BRR, (LL_GPIO_PIN_8 >> GPIO_PIN_MASK_POS) & 0x0000FFFFU)
#define LED0_OFF()		WRITE_REG(GPIOA->BSRR, (LL_GPIO_PIN_8 >> GPIO_PIN_MASK_POS) & 0x0000FFFFU)
#define LED1_ON()		WRITE_REG(GPIOD->BRR, (LL_GPIO_PIN_2 >> GPIO_PIN_MASK_POS) & 0x0000FFFFU)
#define LED1_OFF()		WRITE_REG(GPIOD->BSRR, (LL_GPIO_PIN_2 >> GPIO_PIN_MASK_POS) & 0x0000FFFFU)
#endif

// To Operate LED's GPIO Pin
#define LED0_ON()         HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_OFF()        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#define LED0_TOGGLE()     HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)
#define LED1_ON()         HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF()        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_TOGGLE()     HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)

void LED_GPIO_Init(void);

#endif
