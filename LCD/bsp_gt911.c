#include "bsp_gt911.h"
#include "lcd_init.h"
#include "lcd.h"
#include "bsp_led.h"
#include "stdio.h"

uint8_t touchIC_ID[4];
const uint16_t TPX[] = {0x8150,0x8158,0x8160,0x8168,0x8170}; //电容屏触摸点数据地址（1~5）
const uint16_t POINT_COLOR_TBL[TOUCH_MAX]={RED,GREEN,BLUE,BROWN,GRED};

_m_tp_dev tp_dev=
{
	0,
	0, 
	0,
	0,
};	

void  GT_Init(void)
{
	CTP_GPIOInit();
	CTP_RES_Clr();
	HAL_Delay(10);
	CTP_RES_Set();
	HAL_Delay(10);
	
	CTP_RES_Clr();
	CTP_INT_Clr();
	HAL_Delay(20);
	if(GT_RD_CMD==0x28)
	{
		CTP_INT_Set();
	}
	else
	{
		CTP_INT_Clr();
	}
	HAL_Delay(20);
	CTP_RES_Set();
	HAL_Delay(20);
	GT_RD_DATA(GT_ID_ADDR,4,touchIC_ID);
  LCD_ShowString(0,86,"GT911-ID:",RED,WHITE,24,0);	
  LCD_ShowString(108,86,touchIC_ID,RED,WHITE,24,0);
}
uint8_t GT_WR_DATA(uint16_t addr,uint8_t data)
{
	uint8_t reg;
	CTP_IIC_Start();          //IIC起始信号
	CTP_SendByte(GT_WR_CMD);  //发送写指令
	CTP_WaitAck();    
	CTP_SendByte((uint8_t)(addr>>8));
	CTP_WaitAck();
 	CTP_SendByte((uint8_t)addr);
	CTP_WaitAck();
	CTP_SendByte(data);       //写入数据
	CTP_WaitAck();
	CTP_IIC_Stop();           //IIC结束信号
	reg=SUCCESS;
	return reg;
}
uint8_t GT_RD_DATA(uint16_t addr,uint8_t len,uint8_t *value)
{
  uint8_t reg=ERROR,i;
	CTP_IIC_Start();
	CTP_SendByte(GT_WR_CMD);  //发送写指令
	CTP_WaitAck();    
	CTP_SendByte((uint8_t)(addr>>8));
	CTP_WaitAck();
 	CTP_SendByte((uint8_t)addr);
	CTP_WaitAck();
	
	CTP_IIC_Start();
	IIC_delay(20);
	CTP_SendByte(GT_RD_CMD);
	CTP_WaitAck();
	for(i=0;i<len;i++)
	{
		if(i==(len-1))
		{
			value[i]=CTP_ReadByte(0x00);
		}
		else
		{
			value[i]=CTP_ReadByte(0x01);
		}
	}
	CTP_IIC_Stop();
	reg=SUCCESS;
	return reg;	
}

uint8_t GT911_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	uint8_t tempsta;
 	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{ 
 		GT_RD_DATA(GT_Point_ADDR, 1, &mode);
		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;	
			GT_WR_DATA(GT_Point_ADDR,temp);
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义
			tempsta=tp_dev.sta;			//保存当前的tp_dev.sta值
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//保存触点0的数据
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					GT_RD_DATA(TPX[i],4,buf);	//读取XY坐标值
					if(USE_HORIZONTAL==2)//横屏
					{
						tp_dev.y[i]=240-((uint16_t)buf[1]<<8)-buf[0];
						tp_dev.x[i]=(((uint16_t)buf[3]<<8)+buf[2]);
					}
					else if(USE_HORIZONTAL==0)
					{
						tp_dev.x[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((uint16_t)buf[3]<<8)+buf[2];
					} 
					else if(USE_HORIZONTAL==1)
					{
						tp_dev.x[i]=240-(((uint16_t)buf[1]<<8)+buf[0]);
						tp_dev.y[i]=320-(((uint16_t)buf[3]<<8)+buf[2]);
					} 
					else 
					{
						tp_dev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.x[i]=320-(((uint16_t)buf[3]<<8)+buf[2]);
					} 					
//					printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]>LCD_W||tp_dev.y[0]>LCD_H)//非法数据(坐标超出了)
			{ 
				if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
				}else					//非法数据,则忽略此次数据(还原原来的)
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//恢复tp_dev.sta
				}
			}else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}


//电容触摸屏测试函数

void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{	    
	LCD_DrawPoint(x,y,color);//中心点
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}

void Load_Drow_Dialog(void)
{
	LCD_ShowString(LCD_W-24,0,"RST",RED,WHITE,16,0);//显示清屏区域
}
//////////////////////////////////////////////////////////////////////////////////
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出
	{  
		TP_Draw_Big_Point(uRow,uCol,color);//画点
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   

void GT911_test(void)
{
	uint8_t t=0;
	uint8_t i=0;
 	uint16_t lastpos[5][2];		//最后一次的数据
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	while(1)
	{   
		LCD_ShowString(LCD_W-24,0,"RST",RED,WHITE,16,0);
		GT911_Scan(0);

		for(t=0;t<TOUCH_MAX;t++)//最多5点触摸
		{
			if((tp_dev.sta)&(1<<t))//判断是否有点触摸？
			{    
				HAL_Delay(1);
				if(tp_dev.x[t]<LCD_W&&tp_dev.y[t]<LCD_H)//在LCD范围内
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(LCD_W-24)&&tp_dev.y[t]<16)
					{
						
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		HAL_Delay(5);i++;
	}	
}

//按屏幕上的按键控制LED亮灭
void Button_Scan(void)
{
	uint8_t t=0;

	while(1)
	{
		GT911_Scan(0);

		if((tp_dev.sta)&(1<<t))//判断是否有点触摸？
		{
			HAL_Delay(1);
			if(tp_dev.x[t]>100 && tp_dev.y[t]>100 && tp_dev.x[t]<140 && tp_dev.y[t]<140)//在按钮范围内
			{
				LED0_ON();
				LED1_ON();
			}
			else if(tp_dev.x[t]>100 && tp_dev.y[t]>180 && tp_dev.x[t]<140 && tp_dev.y[t]<220)//在按钮范围内
			{
				LED0_OFF();
				LED1_OFF();
			}
			else
			{}
		}
		HAL_Delay(10);
	}
}

