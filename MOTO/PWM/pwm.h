#ifndef _PWM_H
#define _PWM_H


#include "sys.h"
#include "stm32f10x.h"
void MY_TIM4_GPIO_Init(void);
void MY_TIM4_Init(u16 arr,u16 psc);
void MY_TIM4_PWM_Init(u16 arr,u16 psc);

#endif

