#include "delay.h" 
#include "usart.h"  
#include "HMIsend.h" 


void HMISendstart(void)
	{
	 	delay_ms(200);
		HMISendb(0xff);
		delay_ms(200);
	}
	
void HMISends(char *buf1)		  //�ַ������ͺ���
{
	u8 HMI_i=0;
	while(1)
	{
	 if(buf1[HMI_i]!=0)
	 	{
			USART2_Send(buf1[HMI_i]);  //����һ���ֽ�
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		 	HMI_i++;
		}
	 else 
	 return;
		}
}
void HMISendb(u8 HMI_k)		         //�ֽڷ��ͺ���
{		 
	u8 HMI_ii;
	 for(HMI_ii=0;HMI_ii<3;HMI_ii++)
	 {
	 if(HMI_k!=0)
	 	{
			USART2_Send(HMI_k);  //����һ���ֽ�
			//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
	 else 
	 return ;

	 } 
} 
