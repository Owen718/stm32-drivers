#ifndef __TIMER_H
#define __TIMER_H
#include <stm32f10x_map.h>

/*---------------------------------------��������---------------------------------------------
��ʱ����������
-----------------------------------------��������--------------------------------------------*/
extern u32 TIME2;               //��ʱ��2������
extern u8 flag_time2;			//��ʱ��2��־
extern u8 flag_time2_2;			//��ʱ��2��־
//extern u16 TIME4;               //��ʱ��4������
//extern u16 TIME4;           //���Թ�Ļ���е��޵�ʱ�� �������ٶȣ�����ʹ��
//extern u32 TIME5;               //��ʱ��5������
//extern u32 TIME8;//��ʱ��8������1msһ�Ρ�
/********************************************************************************
*                         ��������                                              *
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























