
#include "sys.h" 
#include "usart.h"     
#include "delay.h"
#include "ds1302.h"                /*DS1302ͷ�ļ�*/
u8 RAM1302_csh=0;    //1302ʱ�ӳ�ʼ������

//u8 time1302_year=0;				           /*��*/
//u8 time1302_month=0;						     /*��*/
//u8 time1302_day=0;							     /*��*/
//u8 time1302_hour=0;							     /*ʱ*/
//u8 time1302_minute=0;						     /*��*/
//u8 time1302_second=0;						     /*��*/

//u8 DS1302_time[10]={0};
//----ds1302�ڲ�����8421BCD����м��㣬���Էֵ�0x55���൱��55�֣�����59�֣��Զ�ת��00��
//----�����ʱ���ʾ��2018��6��1��23ʱ55��0��
u16 year=2018;     //��
u8  year_byte=0;//0x18;
u8 week=0x01;      //��
u8 month=0x06;     //��
u8 day=0x01;       //��
u8 hour=0x23;      //ʱ
u8 minute=0x55;    //��
u8 second=0x00;    //��

//----���յ���λ��ͨ������1��ͬ��ʱ��󣬸ñ�־��1�����������޸ĳ�ʼֵ�󣬱�־����
u32 ds1302_jsq=0;//20180817+�������ȡds1302��ʱ�������Ŀǰ��ѭ��1000�Σ���ȡһ��ʱ����Ϣ

void DS1302IO_Init(void)
{

		RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��
		GPIOB->CRL&=0X0FFFFFFF;
		GPIOB->CRL|=0X30000000;//PB.7 �������
		//GPIOB->ODR|=1<<7;      //PB.7 �����
		//GPIOB->ODR&=0<<7;      //PB.7 �����
		
		GPIOB->CRH&=0XFFFFFF0F;
		GPIOB->CRH|=0X00000030;//PB.9 �������
		//GPIOB->ODR|=1<<9;      //PB.9 �����
		//GPIOB->ODR&=0<<9;      //PB.9 ����� 

		DS1302IO_OUT();

	    DS1302_SCK=0;
		DS1302_CE=0;
		DS1302_O=0;
		delay_ms(10);
}

//��ʼ�� DS1302
void DS1302_TimeInit(void)
{

        second=zh10to_BCD(second);
		minute=zh10to_BCD(minute);
		hour=zh10to_BCD(hour);
		day=zh10to_BCD(day);
		month=zh10to_BCD(month);
		year_byte=zh10to_BCD(year_byte);

        //����ͨ������ÿ�����°����ͬ��ʱ�䣬���Խ�������ж�ȥ��
		//RAM1302_csh=0;
		//RAM1302_csh=Read1302(READ1302_RAM01);
		//if(RAM1302_csh!=0x05)//����������������ܽ��г�ʼ���趨ʱ�䣬��������ȥ��
		{
				Write1302(WRITE_PROTECT,0X00);  //��ֹд����  2098��12��31��23ʱ55��00��

//				Write1302(WRITE_SECOND,0x00);   //��λ��ʼ��
//				Write1302(WRITE_MINUTE,0x55);   //���ӳ�ʼ��
//				Write1302(WRITE_HOUR,0x23);     //Сʱ��ʼ��
//				Write1302(WRITE_DAY,0x31);      //�ճ�ʼ��
//				Write1302(WRITE_MONTH,0x12);    //�³�ʼ��
//				Write1302(WRITE_WEEK,0x04);     //���ճ�ʼ��
//				Write1302(WRITE_YEAR,0x19);     //���ʼ��		
				//Write1302(WRITE1302_RAM01,0x05);//��ʼ����־

                Write1302(WRITE_SECOND,second);   //��λ��ʼ��
				Write1302(WRITE_MINUTE,minute);   //���ӳ�ʼ��
				Write1302(WRITE_HOUR,hour);     //Сʱ��ʼ��
				Write1302(WRITE_DAY,day);      //�ճ�ʼ��
				Write1302(WRITE_MONTH,month);    //�³�ʼ��
				Write1302(WRITE_WEEK,week);     //���ճ�ʼ��
				Write1302(WRITE_YEAR,year_byte);     //���ʼ��		

							
				Write1302(WRITE_PROTECT,0x80);  //����д����
		}
}

//��ַ�����ݷ����ӳ���
void Write1302(u8 addr,u8 dat)
{
		u8 for_i,temp;
		
		DS1302_CE=0;         //CE ����Ϊ�ͣ����ݴ�����ֹ
		DS1302_SCK=0;        //����ʱ������
		DS1302_CE=1;         //CE ����Ϊ�ߣ��߼�������Ч
		//delay_ms(5);
		delay_us(10);
	
		DS1302IO_OUT();
		for(for_i=8;for_i>0;for_i--)   //���͵�ַ  ѭ��8����λ
		{
				temp=addr;
				if(temp&0x01)              //ÿ�δ�����ֽ�
				{
						DS1302_O=1;
				}
				else 
				{
						DS1302_O=0;
				}  
				addr>>=1;                  //����һλ
				delay_us(1);
				DS1302_SCK=1;
				delay_us(1);
				DS1302_SCK=0;
		}
		
		for (for_i=8;for_i>0;for_i-- ) //��������
		{
				
				temp=dat;
				if(temp&0x01)              //ÿ�δ�����ֽ�
				{
						DS1302_O=1;
				}
				else 
				{
						DS1302_O=0;
				}  
				dat>>=1;
				delay_us(1);
				DS1302_SCK=1;
				delay_us(1);
				DS1302_SCK=0;		
		}
		DS1302_CE=0;
		delay_us(2);
}
//���ݶ�ȡ�ӳ���
u8 Read1302(u8 addr)
{
		u8 for_i,temp,dat1,dat2;
		u8 time=0;
	
		DS1302_CE=0;
		DS1302_SCK=0;
		DS1302_CE=1;
		delay_us(10);
		
		DS1302IO_OUT();
		for (for_i=8;for_i>0;for_i--)  //���͵�ַ   ѭ��8����λ
		{
				delay_us(1);
				DS1302_SCK=0;
				temp=addr;
				if(temp&0x01)              //ÿ�δ�����ֽ�
				{
						DS1302_O=1;
				}
				else 
				{
						DS1302_O=0;
				}  
				addr>>=1;                  //����һλ
				delay_us(1);
				DS1302_SCK=1;
		}
		
		DS1302IO_IN();
		for(for_i=7;for_i>0;for_i--)  //��ȡ����
		{		
				DS1302_SCK=0;
				delay_us(1);
				if(DS1302_I)
				{
						time|=0x80;
				}
				time>>=1;
				delay_us(1);
				DS1302_SCK=1;
				delay_us(1);
		}
		DS1302_CE=0;
		dat1=time;
		dat2=dat1/16;  //���ݽ���ת��
		dat1=dat1%16;  //ʮ������תʮ����
		dat1=dat1+dat2*10;
        //----10����ת16����
//		dat2=dat1/10;  //���ݽ���ת��
//		dat1=dat1%10;  //ʮ����תʮ������
//		dat1=dat1+dat2*16;
		return dat1;
}




void read_ds1302time() //��ȡʱ��
{

    second=Read1302(READ_SECOND);   //��ȡ����ʱ��������
	minute=Read1302(READ_MINUTE);   //��ȡ����ʱ��������

	hour=Read1302(READ_HOUR);   //��ȡ����ʱ��������
	day=Read1302(READ_DAY);   //��ȡ����ʱ��������
	month=Read1302(READ_MONTH);   //��ȡ����ʱ��������
	week=Read1302(READ_WEEK);   //��ȡ����ʱ��������
	year_byte=Read1302(READ_YEAER);   //��ȡ����ʱ��������
	year=2000+year_byte;//����ת��ʱ�����
	//USART1_Send(year_byte);
	//USART1_Send(week);
	//USART1_Send(month);
	//USART1_Send(day);
	//USART1_Send(hour);
	//USART1_Send(minute);
	//USART1_Send(second);

}



u8 zh10to_BCD(u8 temp_16) //10����ת��ΪBCD��
{
	u8 zh_i,zh_j,temp_10;
	zh_i=temp_16/10;
	zh_j=temp_16%10;
	temp_10=zh_i*16+zh_j;
	return 	temp_10;
}


















