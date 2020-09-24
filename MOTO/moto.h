#ifndef __MOTO_H
#define __MOTO_H

#include "stm32f10x.h"
//四驱底盘及四轮麦克纳姆轮底盘
//硬件连接说明：
//
//前部电机驱动L298N
//PC6---ENA，控制小车右后电机速度
//PC7---ENB，控制小车左后电机速度
//PC0---IN1，PC1---IN2 控制小车左前电机正反转
//PC2---IN3，PC3---IN4 控制小车右前电机正反转

//后部电机驱动L298N
//PC9---ENB，控制小车右前电机速度
//PC8---ENA，控制小车左前电机速度
//PC10---IN1 ，PC11---IN2  控制小车右后电机的正反转
//PC12---IN3， PD2---IN4   控制小车左后电机的正反转


#define GO    0//定义电机状态 前进
#define BACK  1//后退
#define STOP  2//停车



//配置前部电机驱动的四个引脚
#define MOTO_Q_CLK     RCC_APB2Periph_GPIOC
#define MOTO_Q_PORT    GPIOC
#define MOTO_ZQ1       GPIO_Pin_0  //左前电机第一个控制口
#define MOTO_ZQ2       GPIO_Pin_1  //左前电机第二个控制口
#define MOTO_YQ1       GPIO_Pin_2  //右前电机第一个控制口
#define MOTO_YQ2       GPIO_Pin_3      


//配置后部电机驱动的四个引脚
#define MOTO_H_CLK     RCC_APB2Periph_GPIOC
#define MOTO_H_PORT    GPIOC
#define MOTO_ZH1    GPIO_Pin_12 //左后
#define MOTO_ZH2    GPIO_Pin_2   //PD2左前电机第二个控制口
#define MOTO_YH1    GPIO_Pin_10 //右后
#define MOTO_YH2    GPIO_Pin_11        





void MOTO_GPIO_Config(void);
void MOTO_ZQ(char state);
void MOTO_YQ(char state);
void MOTO_ZH(char state);
void MOTO_YH(char state);
					
void Car_Go(void);
void Car_Back(void);									
void Car_Turn_Right(void);
void Car_Turn_Left(void);
void Car_Stop(void);
void Car_GO_Right(void);
void Car_GO_Left(void);
#endif
