#ifndef _SR501_H
#define _SR501_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

//#define HC_SR501 PAout(0)   //PAI(0)��λ�󶨣���������õģ�

void HC_SR(void);            //��HC_SR501��PA0���ú���

unsigned char HC_SR501_Statue(void);       //��Ϊ���ж�PA0����״̬������������ˣ�˵�����˽�����


#endif


