#include "sht31.h"
#include "delay.h"
#include "IIC.h"
#include "stdio.h"

u8 BUFt[2]={0};
u8 BUFh[2]={0};
u8 rxbufsht31[6];
uint8_t tem ;
uint8_t hum ;


void Cmd_Write_sht31(uint8_t msb,uint8_t lsb)
{
  IIC_Start();                  	//��ʼ�ź�
  IIC_Send_Byte(0x88+0);   				//�����豸��ַ+д�ź�
	while(IIC_Wait_Ack());					//�ӻ�Ӧ��

  IIC_Send_Byte(msb);    					//��������
	while(IIC_Wait_Ack());

	IIC_Send_Byte(lsb);    					//��������
	while(IIC_Wait_Ack());

  IIC_Stop();                   	//����ֹͣ�ź�
	delay_ms(5);
}


/*�����������ȷ����¶�ֵ��Ȼ�������ʪ��ֵ�����յ�ʪ��ֵ��У��ͺ󣬾�Ӧ�÷���һ��NACK��ֹͣ����*/
void Read_sht31(void)
{   	
		u8 i;
		Cmd_Write_sht31(0x21,0x26); 	//���ݲɼ�Ƶ��1 mps�����������ݲɼ�ģʽ�Ĳ�������
		IIC_Start();   	//��ʼ�ź�
    IIC_Send_Byte(0x88+1);        //�����豸��ַ+���ź�
		while(IIC_Wait_Ack());
		delay_us(5);
		for(i=0;i<6;i++)
		{
			if(i==5)*(rxbufsht31+i)=IIC_Read_Byte(0);	//��ȡʪ��У�����NACK
			else *(rxbufsht31+i)=IIC_Read_Byte(1);		//��ȡ������ACK
		}
    IIC_Stop();                          				//����ֹͣ�ź�
		BUFt[0]=rxbufsht31[0];  										//�����¶�����
		BUFt[1]=rxbufsht31[1];  
	
		BUFh[0]=rxbufsht31[3];  										//����ʪ������
		BUFh[1]=rxbufsht31[4];  
    delay_ms(5);
}

//����ת��
//void Convert_sht31(void)
//{
//	tem=175*(BUFt[0]*256+BUFt[1])/65535-45; //���϶�
//	hum=100*(BUFh[0]*256+BUFh[1])/65535;
//	printf("��ǰ�¶ȣ� %d\r\n",tem);
//	printf("��ǰʪ�ȣ� %d\r\n",hum);
//	BUFt[0]=0;
//	BUFt[1]=0;
//	BUFh[0]=0;
//	BUFh[1]=0;
//}
int Convert_sht31_tem(void)
{
	tem=175*(BUFt[0]*256+BUFt[1])/65535-45; //���϶�
	BUFt[0]=0;
	BUFt[1]=0;
	return tem;
}

int Convert_sht31_hum(void)
{
	hum=100*(BUFh[0]*256+BUFh[1])/65535;
	BUFh[0]=0;
	BUFh[1]=0;
	return hum;
}

