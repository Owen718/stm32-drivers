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
  IIC_Start();                  	//起始信号
  IIC_Send_Byte(0x88+0);   				//发送设备地址+写信号
	while(IIC_Wait_Ack());					//从机应答

  IIC_Send_Byte(msb);    					//发送命令
	while(IIC_Wait_Ack());

	IIC_Send_Byte(lsb);    					//发送命令
	while(IIC_Wait_Ack());

  IIC_Stop();                   	//发送停止信号
	delay_ms(5);
}


/*传感器将首先发送温度值，然后发送相对湿度值。在收到湿度值的校验和后，就应该发送一个NACK和停止条件*/
void Read_sht31(void)
{   	
		u8 i;
		Cmd_Write_sht31(0x21,0x26); 	//数据采集频率1 mps，周期性数据采集模式的测量命令
		IIC_Start();   	//起始信号
    IIC_Send_Byte(0x88+1);        //发送设备地址+读信号
		while(IIC_Wait_Ack());
		delay_us(5);
		for(i=0;i<6;i++)
		{
			if(i==5)*(rxbufsht31+i)=IIC_Read_Byte(0);	//读取湿度校验后发送NACK
			else *(rxbufsht31+i)=IIC_Read_Byte(1);		//读取并发送ACK
		}
    IIC_Stop();                          				//发送停止信号
		BUFt[0]=rxbufsht31[0];  										//存入温度数据
		BUFt[1]=rxbufsht31[1];  
	
		BUFh[0]=rxbufsht31[3];  										//存入湿度数据
		BUFh[1]=rxbufsht31[4];  
    delay_ms(5);
}

//数据转换
//void Convert_sht31(void)
//{
//	tem=175*(BUFt[0]*256+BUFt[1])/65535-45; //摄氏度
//	hum=100*(BUFh[0]*256+BUFh[1])/65535;
//	printf("当前温度： %d\r\n",tem);
//	printf("当前湿度： %d\r\n",hum);
//	BUFt[0]=0;
//	BUFt[1]=0;
//	BUFh[0]=0;
//	BUFh[1]=0;
//}
int Convert_sht31_tem(void)
{
	tem=175*(BUFt[0]*256+BUFt[1])/65535-45; //摄氏度
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

