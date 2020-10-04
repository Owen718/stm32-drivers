#include "30205.h" 
#include "delay.h" 
//float temperature=0;
 void shutdown(void)
 {
 
 
 }
 void max_init (void)
 {
	 IIC_Init();
   I2CwriteByte(MAX30205_ADDRESS, MAX30205_CONFIGURATION, 0x00);
	 //I2CwriteByte(MAX30205_ADDRESS, MAX30205_THYST ,        0x00);
  // I2CwriteByte(MAX30205_ADDRESS, MAX30205_TOS, 0x00);
 } 
// float GetTemperature(void)
// {  
//	  float temperature1=0.0;
//    u8 readRaw[2]={0,0};
//	  u16 raw=0;
//	  u16 raw1=0;
//	  I2CreadBytes(MAX30205_ADDRESS,MAX30205_TEMPERATURE, &readRaw[0] ,2);
//	  raw=raw|readRaw[0];
//    raw1=(raw<<8)|readRaw[1];	 
//    temperature1 = raw1  * 0.00390625+2.0;     // convert to temperature
//	  return  temperature1;
// 
// }
 
 double GetTemperature(void)
	{
		double temperature= 0.0;
		int16_t raw = 0;
		uint8_t readRaw[2];
    I2CreadBytes(MAX30205_ADDRESS,MAX30205_TEMPERATURE, readRaw ,2); // read two bytes
		printf("readRaw[0];:%d\r\n",readRaw[0]);
		printf("readRaw[1];:%d\r\n",readRaw[1]);
		raw = readRaw[0] << 8 | readRaw[1];  //combine two bytes
    temperature = raw  * 0.00390625;     // convert to temperature
		printf("raw:%d\r\n",raw);
		return  temperature;
	}
	
 //写入指定地址一个字节数据
 void I2CwriteByte(u8 address,u8 subAddress,u8 data)  //
 {
     //IIC_Start();                                       //产生开始信号
	   I2Cwriteaddr(address|IIC_WRITE,subAddress);  //对器件地址写入命令信号 然后寄存器地址
	   IIC_Send_Byte(data);                               //对数据进行写进去
	   IIC_Wait_Ack();                                    //等待回应
	   IIC_Stop();	                         //产生停止信号
}
 //I2C读一个字节操作
 u8 I2CreadByte(u8 address,u8 subAddress)
 {   
	   u8 data;
    // IIC_Start();                           //开始
	   I2Cwriteaddr(address|IIC_WRITE, subAddress); //对器件地址发送写入命令 写入寄存器地址
     IIC_Start(); 	                                      //开始
	   IIC_Send_Byte(address|IIC_READ);                //发送读取命令
	 
	   IIC_Wait_Ack();                                    //等待回应
	   data=IIC_Read_Byte(0);
	   IIC_Stop();	                         //产生停止信号
	   return data;	   
}
 
 void I2CreadBytes(u8 address,u8 subAddress,u8*dest,u8 count)
 {
    int i;
	  for(i=0;i<count;i++)
		{
			dest[i]=I2CreadByte(address,subAddress);
			subAddress+=1;
		}
 
}
 

void I2Cwriteaddr(u8 address,u8 subAddress)//写地址和寄存器操作
{
     IIC_Start();                         //产生开始条件
	   IIC_Send_Byte(address);              //写入器件地址
	   IIC_Wait_Ack();                      //等待回应
	   IIC_Send_Byte(subAddress);           //写入寄存器地址
	   IIC_Wait_Ack();                       //等待回应
	   //IIC_Stop();	                         //产生停止信号
}
int16_t Read_max16(u8 address,u8 subAddress)
{
			int16_t data;
			u8 instant;
		// IIC_Start();                           //开始
			I2Cwriteaddr(address|IIC_WRITE, subAddress); //对器件地址发送写入命令 写入寄存器地址
			IIC_Start(); 	                                      //开始
			IIC_Send_Byte(address|IIC_READ);                //发送读取命令
		 
			IIC_Wait_Ack();                                    //等待回应
			data=IIC_Read_Byte(1);
			//IIC_Wait_Ack(); 
			instant = IIC_Read_Byte(0);
			IIC_Stop();	                         //产生停止信号
			data = (data<<8)|instant;
			return data;	
}	

double GetTemperature1(void)
	{
		double temperature= 0.0;
		int16_t temp;
		temp = Read_max16(MAX30205_ADDRESS,MAX30205_TEMPERATURE);
		temperature = temp*0.00390625;
		return  temperature;
	}








