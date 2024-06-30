#ifndef __BSP_TB6642_H
#define __BSP_TB6642_H

#include "main.h"
#include "tim.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  MOTOR_DIR_CCW = 0,
  MOTOR_DIR_CW,
}MotorDir_Typedef;

typedef enum {
  MOTOR_ENABLE = ENABLE,
  MOTOR_DISABLE = DISABLE,
}MotorSta_Typedef;




/* Private defines -----------------------------------------------------------*/
#define TB6642_IN1_PIN GPIO_PIN_9
#define TB6642_IN2_PIN GPIO_PIN_10
#define TB6642_GPIO_Port GPIOA



/* Exported functions prototypes ---------------------------------------------*/
void TB6642_GPIO_Init(void);
void TB6642_BreakToStop(void);
void TB6642_High_Resistance(void);
void TB6642_SetSpeed(uint16_t duty);
void TB6642_SetMotorDir(MotorDir_Typedef dir);
void TB6642_SetMotorEna(MotorSta_Typedef ena);


#endif



