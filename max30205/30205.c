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
	
 //д��ָ����ַһ���ֽ�����
 void I2CwriteByte(u8 address,u8 subAddress,u8 data)  //
 {
     //IIC_Start();                                       //������ʼ�ź�
	   I2Cwriteaddr(address|IIC_WRITE,subAddress);  //��������ַд�������ź� Ȼ��Ĵ�����ַ
	   IIC_Send_Byte(data);                               //�����ݽ���д��ȥ
	   IIC_Wait_Ack();                                    //�ȴ���Ӧ
	   IIC_Stop();	                         //����ֹͣ�ź�
}
 //I2C��һ���ֽڲ���
 u8 I2CreadByte(u8 address,u8 subAddress)
 {   
	   u8 data;
    // IIC_Start();                           //��ʼ
	   I2Cwriteaddr(address|IIC_WRITE, subAddress); //��������ַ����д������ д��Ĵ�����ַ
     IIC_Start(); 	                                      //��ʼ
	   IIC_Send_Byte(address|IIC_READ);                //���Ͷ�ȡ����
	 
	   IIC_Wait_Ack();                                    //�ȴ���Ӧ
	   data=IIC_Read_Byte(0);
	   IIC_Stop();	                         //����ֹͣ�ź�
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
 

void I2Cwriteaddr(u8 address,u8 subAddress)//д��ַ�ͼĴ�������
{
     IIC_Start();                         //������ʼ����
	   IIC_Send_Byte(address);              //д��������ַ
	   IIC_Wait_Ack();                      //�ȴ���Ӧ
	   IIC_Send_Byte(subAddress);           //д��Ĵ�����ַ
	   IIC_Wait_Ack();                       //�ȴ���Ӧ
	   //IIC_Stop();	                         //����ֹͣ�ź�
}
int16_t Read_max16(u8 address,u8 subAddress)
{
			int16_t data;
			u8 instant;
		// IIC_Start();                           //��ʼ
			I2Cwriteaddr(address|IIC_WRITE, subAddress); //��������ַ����д������ д��Ĵ�����ַ
			IIC_Start(); 	                                      //��ʼ
			IIC_Send_Byte(address|IIC_READ);                //���Ͷ�ȡ����
		 
			IIC_Wait_Ack();                                    //�ȴ���Ӧ
			data=IIC_Read_Byte(1);
			//IIC_Wait_Ack(); 
			instant = IIC_Read_Byte(0);
			IIC_Stop();	                         //����ֹͣ�ź�
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








