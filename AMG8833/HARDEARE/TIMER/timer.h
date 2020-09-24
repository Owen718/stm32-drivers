#ifndef __TIMER_H
#define __TIMER_H
#include <stm32f10x_map.h>

/*---------------------------------------变量声明---------------------------------------------
定时器变量声明
-----------------------------------------变量声明--------------------------------------------*/
extern u32 TIME2;               //定时器2计数器
extern u8 flag_time2;			//定时器2标志
extern u8 flag_time2_2;			//定时器2标志
//extern u16 TIME4;               //定时器4计数器
//extern u16 TIME4;           //测试光幕从有到无的时间 ，计算速度，测试使用
//extern u32 TIME5;               //定时器5计数器
//extern u32 TIME8;//定时器8计数，1ms一次。
/********************************************************************************
*                         函数声明                                              *
********************************************************************************/
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
//void TIM4_Int_Init(u16 arr,u16 psc);
//void TIM5_Int_Init(u16 arr,u16 psc);
//void TIM6_Int_Init(u16 arr,u16 psc);
//void TIM7_Int_Init(u16 arr,u16 psc);

//void TIM8_Int_Init(u16 arr,u16 psc);

//void TIM1_Int_Init(u16 arr,u16 psc);
#endif























