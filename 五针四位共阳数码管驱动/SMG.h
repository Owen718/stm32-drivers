#ifndef _SMG_H_
#define _SMG_H_

#include "stm32f10x.h"


//五脚四位数码管： VCC GND DIO SCLK RCLK


#define DIO   GPIO_Pin_8   // DIO   PC8
#define SCLK  GPIO_Pin_6   // SCLK  PC6
#define RCLK   GPIO_Pin_7   // RCLK PC7


#define SET_DIO  GPIO_SetBits(GPIOC,DIO)//置1
#define CLR_DIO  GPIO_ResetBits(GPIOC,DIO)//清0

#define SET_SCLK  GPIO_SetBits(GPIOC,SCLK)
#define CLR_SCLK  GPIO_ResetBits(GPIOC,SCLK)


#define SET_RCLK  GPIO_SetBits(GPIOC,RCLK)
#define CLR_RCLK  GPIO_ResetBits(GPIOC,RCLK)


void HC595_Init(void);
void HC595_WR(unsigned char dta);
void HC595_Over(void);
void SMG_Display(unsigned char data,unsigned char wel);
void key1(void);
void key2(void);


#endif




