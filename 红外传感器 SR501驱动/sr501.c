#include  "sr501.h"
#include "stm32f10x.h"
#include "sys.h"

void HC_SR(void)
{
		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);//打开GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选中0引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//配置为下拉输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
}

unsigned char HC_SR501_Statue(void)
{
	if(PAin(0)==1)//如果GPIOA_0 引脚输入高电平，代表有人，返回1
	{
	  return  1;
	}
    return 0;
}




