#ifndef __BSP_GT911_H_
#define __BSP_GT911_H_

#include "i2c.h"


#define TOUCH_MAX 5

#define GT_WR_CMD 0xBA  		//写命令地址
#define GT_RD_CMD 0xBB  		//读数据地址

#define GT_Point_ADDR 0x814E	//GT911当前检测到的触摸情况，第七位是触摸标志位，低4位是触摸点数个数
#define GT_ID_ADDR    0x8140	//GT911产品ID寄存器

#define TP_PRES_DOWN 0x80  		//触屏被按下
#define TP_CATH_PRES 0x40  		//有按键按下了


//触摸屏控制器
typedef struct 
{
	uint16_t x[TOUCH_MAX]; 	//当前坐标
	uint16_t y[TOUCH_MAX];	//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[4],y[4]存储第一次按下时的坐标.
	uint8_t  sta;		  //笔的状态
								//b7:按下1/松开0;
	              //b6:0,没有按键按下;1,有按键按下.
								//b5:保留
	uint8_t touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义


#define CTP_INT_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)//INT
#define CTP_INT_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)

#define CTP_RES_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)//RES
#define CTP_RES_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)

void CTP_GPIOInit(void);
uint8_t GT_WR_DATA(uint16_t addr,uint8_t data);
uint8_t GT_RD_DATA(uint16_t addr,uint8_t len,uint8_t *value);
void  GT_Init(void);

uint8_t GT911_Scan(uint8_t mode);

void Load_Drow_Dialog(void);
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);

void GT911_test(void);

void GT911_Write_Data(uint16_t addr,uint8_t data);
void GT911_Read_Data(uint16_t addr,uint8_t len,uint8_t *value);

#endif

