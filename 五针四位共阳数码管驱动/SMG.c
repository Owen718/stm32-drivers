#include "SMG.h"
#include "stm32f10x.h"                  // Device header
#include "delay.h"

/*
const unsigned char LED_table[24]={
//01234567
0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
//89ABCDEF
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,
//ʱ��Ч��
0xF9,0xBF,0x99,0xBF,0x82,0xF9,0xC0,0xA4
};

//��������ܶ��롰0123456789AbCdEFϨ��-��
const unsigned char wei_table[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//��һλ���ڰ�λλ��
*/


/*ʹ�÷�����
//��λ��������ܣ�2��74HC595����
SMG_Display(LED_table[0],wei_table[1]); ��1λ��ʾ����0 
SMG_Display(LED_table[1] & 0x7f,wei_table[2]);  ��2λ��ʾ���� 1.  
SMG_Display(LED_table[2],wei_table[3]);   ��3λ��ʾ���� 2
*/





void HC595_Init()  //��ʼ������
{
		
	GPIO_InitTypeDef IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //GPIOC ���ų�ʼ��
	IO.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
	IO.GPIO_Speed = GPIO_Speed_50MHz;  
	IO.GPIO_Pin = DIO|SCLK|RCLK;
	GPIO_Init(GPIOC,&IO);

}

void HC595_WR(unsigned char data)
{
	u8 i;
	for(i = 0;i<8;i++)
	{
		if(data&0x80)//��һȡ���λ
			SET_DIO;//д1
		else
			CLR_DIO;//д0
		
			data<<=1;//���θ�λ��Ϊ���λ
			CLR_SCLK;  //ʱ�������� 0 
			SET_SCLK;//ʱ��������   1
	}
}

void HC595_Over()//����RCLKʱ��������
{

		CLR_RCLK;   //0
		SET_RCLK;   //1
		
}



void SMG_Display(unsigned char data,unsigned char wei)  //��ʾ���� data���������ʾ���ݣ�wei����һλ
{

	HC595_WR(data); //�ͳ�����
	HC595_WR(wei);//�ͳ�λ��
	HC595_Over(); //�������
	delay_ms(2);

}

