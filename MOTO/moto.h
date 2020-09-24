#ifndef __MOTO_H
#define __MOTO_H

#include "stm32f10x.h"
//�������̼����������ķ�ֵ���
//Ӳ������˵����
//
//ǰ���������L298N
//PC6---ENA������С���Һ����ٶ�
//PC7---ENB������С��������ٶ�
//PC0---IN1��PC1---IN2 ����С����ǰ�������ת
//PC2---IN3��PC3---IN4 ����С����ǰ�������ת

//�󲿵������L298N
//PC9---ENB������С����ǰ����ٶ�
//PC8---ENA������С����ǰ����ٶ�
//PC10---IN1 ��PC11---IN2  ����С���Һ���������ת
//PC12---IN3�� PD2---IN4   ����С�������������ת


#define GO    0//������״̬ ǰ��
#define BACK  1//����
#define STOP  2//ͣ��



//����ǰ������������ĸ�����
#define MOTO_Q_CLK     RCC_APB2Periph_GPIOC
#define MOTO_Q_PORT    GPIOC
#define MOTO_ZQ1       GPIO_Pin_0  //��ǰ�����һ�����ƿ�
#define MOTO_ZQ2       GPIO_Pin_1  //��ǰ����ڶ������ƿ�
#define MOTO_YQ1       GPIO_Pin_2  //��ǰ�����һ�����ƿ�
#define MOTO_YQ2       GPIO_Pin_3      


//���ú󲿵���������ĸ�����
#define MOTO_H_CLK     RCC_APB2Periph_GPIOC
#define MOTO_H_PORT    GPIOC
#define MOTO_ZH1    GPIO_Pin_12 //���
#define MOTO_ZH2    GPIO_Pin_2   //PD2��ǰ����ڶ������ƿ�
#define MOTO_YH1    GPIO_Pin_10 //�Һ�
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
