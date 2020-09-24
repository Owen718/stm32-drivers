#include "pwm.h"




//***************** 定时器PWM输出初始化函数
void MY_TIM4_PWM_Init(u16 arr,u16 psc)
{ 
	GPIO_InitTypeDef GPIO_InitStructrue;
    TIM_TimeBaseInitTypeDef TIM_TImeBaseInitstructrue;
	TIM_OCInitTypeDef TIM_OCInitstructure;
	
	//使能GPIOB.8 GPIOB.9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //开启时钟
	
	
	GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_8;   
	GPIO_InitStructrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructrue); 

    GPIO_InitStructrue.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructrue);    
	

	TIM_TImeBaseInitstructrue.TIM_Prescaler=psc;
	TIM_TImeBaseInitstructrue.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TImeBaseInitstructrue.TIM_Period=arr;
	TIM_TImeBaseInitstructrue.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TImeBaseInitstructrue);
	TIM_Cmd(TIM4,ENABLE);

		
	TIM_OCInitstructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitstructure.TIM_OutputNState=TIM_OutputNState_Disable;
	
	TIM_OC3Init(TIM4,&TIM_OCInitstructure);
	TIM_OC4Init(TIM4,&TIM_OCInitstructure);
	
	
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	

}

