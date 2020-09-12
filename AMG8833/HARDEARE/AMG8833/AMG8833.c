#include "AMG8833.h" 
#include "delay.h" 
#include "usart.h"    
//float temperature=0;

float temp;
float print_buf[64];
float print_buf_max[64];
float temprature=0.00; 
u8 result1=0;
u8 result2=0;
u8 temp1=0X69<<1;
u8 temp2=(0X69<<1)|0x01;	
u16 result=0;
//u16 reg1=0;
u8 i=0;

u8 pixel_buf[128]={0};
//u8 test_mod=none;
//long ext[3]={0,0,0};
//u8 ext_add[2];
//u16 data[40][40];

u8 Init_AMG8833(void){   //×ÜÐÅºÅ£º 0xD2 0X00 0X00 0XD2 0x01 0x3f 0xD2 0x02 0x00
	IIC_Start();   //I2C³õÊ¼»¯
	IIC_Send_Byte(0xD2);  //·¢ËÍÔ¤ÉèÖµ
	if(IIC_Wait_Ack())return 0;  //µÈ´ýÓ¦´ðÐÅºÅ£¬Ó¦´ðÊ§°Ü½áÊø³õÊ¼»¯
	IIC_Send_Byte(0x00);  //I2C·¢ËÍÉè¶¨Öµ
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(0x00);
	if(IIC_Wait_Ack())return 0;
	IIC_Stop();  //²úÉúI2C½áÊøÐÅºÅ
	delay_ms(10);  //ÑÓÊ±10ms
	IIC_Start();
	IIC_Send_Byte(0xD2);  
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(0x01);
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(0x3f);
	if(IIC_Wait_Ack())return 0;
	IIC_Stop();
	delay_ms(10);
	IIC_Start();
	IIC_Send_Byte(0xD2);
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(0x02);
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(0x00);
	if(IIC_Wait_Ack())return 0;
	IIC_Stop();

	delay_ms(50);
	//get_data();
	return 1;
}

void read_onepixel()   //¶ÁÈ¡µ¥¸öÏñËØ
{
	  IIC_Start(); 
		IIC_Send_Byte(temp1);	    //·¢ËÍÐ´ÃüÁî   temp1=0X69<<1;
		IIC_Wait_Ack();
		IIC_Send_Byte(0XC0);	    //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		
		IIC_Start(); 
		IIC_Send_Byte(temp2);	    //·¢ËÍÐ´ÃüÁî   temp2=(0X69<<1)|0x01;
		IIC_Wait_Ack();
		
		result1=IIC_Read_Byte(0);

		IIC_NAck();
		IIC_Stop();
		
		delay_ms(1);
		
		IIC_Start(); 
		IIC_Send_Byte(temp1);	    //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		IIC_Send_Byte(0XC1);	    //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		
		IIC_Start(); 
		IIC_Send_Byte(temp2);	    //·¢ËÍÐ´ÃüÁî
		IIC_Wait_Ack();
		
		result2=IIC_Read_Byte(0);

		IIC_NAck();
		IIC_Stop();
		
		result=result2<<8;
		result=result|result1;
		
		result=result&0X7FF;
		temprature=(float)result*0.25;
		printf("%f\r\n",temprature);
}

void read_64pixel()   //¶ÁÈ¡8*8ÏñËØ
{
	IIC_Start(); 
	IIC_Send_Byte(temp1);	    //·¢ËÍÐ´ÃüÁî
	IIC_Wait_Ack();
	IIC_Send_Byte(0X80);	    //·¢ËÍÐ´ÃüÁî
	IIC_Wait_Ack();

	IIC_Start(); 
	IIC_Send_Byte(temp2);	    //·¢ËÍÐ´ÃüÁî
	IIC_Wait_Ack();

	for(i=0;i<128;i++)
	{
		if(i==127)
		{
			pixel_buf[i]=IIC_Read_Byte(0);
		}
		else
		{
			pixel_buf[i]=IIC_Read_Byte(1);
		}	
	}
	
	IIC_NAck();
	IIC_Stop();
	
}

float change(u8 x)
{
	u16 tempx;
	float tempraturex;
	tempx=pixel_buf[x]<<8;
	tempx=tempx|pixel_buf[x-1];
		
	tempx=tempx&0X7FF;
	tempraturex=(float)tempx*0.2;
	return tempraturex;
//	printf("%f\r\n",tempraturex);
}

void get_64pixel()
{
	u8 m=0;
	u8 i=0;
	for(m=1;m<128;m=m+2)
	{
		print_buf[i]=change(m);
		print_buf_max[i]=print_buf[i];
		i++;
		
	}
}

void printf_array()
{
	u8 i=0;
	u8 size=63;
	get_64pixel();
	for(i=1;i<65;i++)
	{
		printf("[%.2f]  ",print_buf[size]);
		if(i%8==0)
		{
			printf("\r\n");
		}
		size=size-1;
	}
}
