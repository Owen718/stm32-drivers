#include  "sr501.h"
#include "stm32f10x.h"
#include "sys.h"

void HC_SR(void)
{
		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);//��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//ѡ��0����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
}

unsigned char HC_SR501_Statue(void)
{
	if(PAin(0)==1)//���GPIOA_0 ��������ߵ�ƽ���������ˣ�����1
	{
	  return  1;
	}
    return 0;
}




