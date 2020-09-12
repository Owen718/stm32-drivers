#include "delay.h" 
#include "usart.h"  
#include "HMIsend.h" 


void HMISendstart(void)
	{
	 	delay_ms(200);
		HMISendb(0xff);
		delay_ms(200);
	}
	
void HMISends(char *buf1)		  //字符串发送函数
{
	u8 HMI_i=0;
	while(1)
	{
	 if(buf1[HMI_i]!=0)
	 	{
			USART2_Send(buf1[HMI_i]);  //发送一个字节
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	HMI_i++;
		}
	 else 
	 return;
		}
}
void HMISendb(u8 HMI_k)		         //字节发送函数
{		 
	u8 HMI_ii;
	 for(HMI_ii=0;HMI_ii<3;HMI_ii++)
	 {
	 if(HMI_k!=0)
	 	{
			USART2_Send(HMI_k);  //发送一个字节
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
	 return ;

	 } 
} 
