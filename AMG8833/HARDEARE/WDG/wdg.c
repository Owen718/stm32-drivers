#include "main.h"
/*************************************************************************
函数名称: void IWDG_Init(void) 
函数说明:看门狗初始化程序
函数功能:
函数变量:
注    意:
*************************************************************************/
//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG->KR=0X5555;//使能对IWDG->PR和IWDG->RLR的写		 										  
  	IWDG->PR=prer;  //设置分频系数   
  	IWDG->RLR=rlr;  //从加载寄存器 IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//使能看门狗	
}
/*************************************************************************
函数名称: void IWDG_Feed(void) 
函数说明:看门狗喂狗程序
函数功能:
函数变量:
注    意:
*************************************************************************/
//喂独立看门狗
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}








