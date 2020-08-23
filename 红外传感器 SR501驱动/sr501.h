#ifndef _SR501_H
#define _SR501_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

//#define HC_SR501 PAout(0)   //PAI(0)是位绑定，方便操作用的，

void HC_SR(void);            //是HC_SR501的PA0配置函数

unsigned char HC_SR501_Statue(void);       //是为了判断PA0输入状态，如果被拉高了，说明有人进入了


#endif


