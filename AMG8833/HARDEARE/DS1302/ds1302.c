
#include "sys.h" 
#include "usart.h"     
#include "delay.h"
#include "ds1302.h"                /*DS1302头文件*/
u8 RAM1302_csh=0;    //1302时钟初始化数据

//u8 time1302_year=0;				           /*年*/
//u8 time1302_month=0;						     /*月*/
//u8 time1302_day=0;							     /*日*/
//u8 time1302_hour=0;							     /*时*/
//u8 time1302_minute=0;						     /*分*/
//u8 time1302_second=0;						     /*秒*/

//u8 DS1302_time[10]={0};
//----ds1302内部采用8421BCD码进行计算，所以分的0x55就相当于55分，到了59分，自动转换00，
//----下面的时间表示：2018年6月1日23时55分0秒
u16 year=2018;     //年
u8  year_byte=0;//0x18;
u8 week=0x01;      //周
u8 month=0x06;     //月
u8 day=0x01;       //日
u8 hour=0x23;      //时
u8 minute=0x55;    //分
u8 second=0x00;    //秒

//----接收到上位机通过串口1的同步时间后，该标志置1，在主程序修改初始值后，标志清零
u32 ds1302_jsq=0;//20180817+主程序读取ds1302的时间次数，目前是循环1000次，读取一次时间信息

void DS1302IO_Init(void)
{

		RCC->APB2ENR|=1<<3;    	//使能PORTB时钟
		GPIOB->CRL&=0X0FFFFFFF;
		GPIOB->CRL|=0X30000000;//PB.7 推挽输出
		//GPIOB->ODR|=1<<7;      //PB.7 输出高
		//GPIOB->ODR&=0<<7;      //PB.7 输出高
		
		GPIOB->CRH&=0XFFFFFF0F;
		GPIOB->CRH|=0X00000030;//PB.9 推挽输出
		//GPIOB->ODR|=1<<9;      //PB.9 输出高
		//GPIOB->ODR&=0<<9;      //PB.9 输出高 

		DS1302IO_OUT();

	    DS1302_SCK=0;
		DS1302_CE=0;
		DS1302_O=0;
		delay_ms(10);
}

//初始化 DS1302
void DS1302_TimeInit(void)
{

        second=zh10to_BCD(second);
		minute=zh10to_BCD(minute);
		hour=zh10to_BCD(hour);
		day=zh10to_BCD(day);
		month=zh10to_BCD(month);
		year_byte=zh10to_BCD(year_byte);

        //这里通过串口每天上下班更新同步时间，所以将下面的判断去了
		//RAM1302_csh=0;
		//RAM1302_csh=Read1302(READ1302_RAM01);
		//if(RAM1302_csh!=0x05)//加上这个条件，不能进行初始化设定时间，所以这里去掉
		{
				Write1302(WRITE_PROTECT,0X00);  //禁止写保护  2098年12月31日23时55分00秒

//				Write1302(WRITE_SECOND,0x00);   //秒位初始化
//				Write1302(WRITE_MINUTE,0x55);   //分钟初始化
//				Write1302(WRITE_HOUR,0x23);     //小时初始化
//				Write1302(WRITE_DAY,0x31);      //日初始化
//				Write1302(WRITE_MONTH,0x12);    //月初始化
//				Write1302(WRITE_WEEK,0x04);     //周日初始化
//				Write1302(WRITE_YEAR,0x19);     //年初始化		
				//Write1302(WRITE1302_RAM01,0x05);//初始化标志

                Write1302(WRITE_SECOND,second);   //秒位初始化
				Write1302(WRITE_MINUTE,minute);   //分钟初始化
				Write1302(WRITE_HOUR,hour);     //小时初始化
				Write1302(WRITE_DAY,day);      //日初始化
				Write1302(WRITE_MONTH,month);    //月初始化
				Write1302(WRITE_WEEK,week);     //周日初始化
				Write1302(WRITE_YEAR,year_byte);     //年初始化		

							
				Write1302(WRITE_PROTECT,0x80);  //允许写保护
		}
}

//地址、数据发送子程序
void Write1302(u8 addr,u8 dat)
{
		u8 for_i,temp;
		
		DS1302_CE=0;         //CE 引脚为低，数据传送中止
		DS1302_SCK=0;        //清零时钟总线
		DS1302_CE=1;         //CE 引脚为高，逻辑控制有效
		//delay_ms(5);
		delay_us(10);
	
		DS1302IO_OUT();
		for(for_i=8;for_i>0;for_i--)   //发送地址  循环8次移位
		{
				temp=addr;
				if(temp&0x01)              //每次传输低字节
				{
						DS1302_O=1;
				}
				else 
				{
						DS1302_O=0;
				}  
				addr>>=1;                  //右移一位
				delay_us(1);
				DS1302_SCK=1;
				delay_us(1);
				DS1302_SCK=0;
		}
		
		for (for_i=8;for_i>0;for_i-- ) //发送数据
		{
				
				temp=dat;
				if(temp&0x01)              //每次传输低字节
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
//数据读取子程序
u8 Read1302(u8 addr)
{
		u8 for_i,temp,dat1,dat2;
		u8 time=0;
	
		DS1302_CE=0;
		DS1302_SCK=0;
		DS1302_CE=1;
		delay_us(10);
		
		DS1302IO_OUT();
		for (for_i=8;for_i>0;for_i--)  //发送地址   循环8次移位
		{
				delay_us(1);
				DS1302_SCK=0;
				temp=addr;
				if(temp&0x01)              //每次传输低字节
				{
						DS1302_O=1;
				}
				else 
				{
						DS1302_O=0;
				}  
				addr>>=1;                  //右移一位
				delay_us(1);
				DS1302_SCK=1;
		}
		
		DS1302IO_IN();
		for(for_i=7;for_i>0;for_i--)  //读取数据
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
		dat2=dat1/16;  //数据进制转换
		dat1=dat1%16;  //十六进制转十进制
		dat1=dat1+dat2*10;
        //----10进制转16进制
//		dat2=dat1/10;  //数据进制转换
//		dat1=dat1%10;  //十进制转十六进制
//		dat1=dat1+dat2*16;
		return dat1;
}




void read_ds1302time() //读取时间
{

    second=Read1302(READ_SECOND);   //读取分秒时日月周年
	minute=Read1302(READ_MINUTE);   //读取分秒时日月周年

	hour=Read1302(READ_HOUR);   //读取分秒时日月周年
	day=Read1302(READ_DAY);   //读取分秒时日月周年
	month=Read1302(READ_MONTH);   //读取分秒时日月周年
	week=Read1302(READ_WEEK);   //读取分秒时日月周年
	year_byte=Read1302(READ_YEAER);   //读取分秒时日月周年
	year=2000+year_byte;//给年转换时间计算
	//USART1_Send(year_byte);
	//USART1_Send(week);
	//USART1_Send(month);
	//USART1_Send(day);
	//USART1_Send(hour);
	//USART1_Send(minute);
	//USART1_Send(second);

}



u8 zh10to_BCD(u8 temp_16) //10进制转换为BCD码
{
	u8 zh_i,zh_j,temp_10;
	zh_i=temp_16/10;
	zh_j=temp_16%10;
	temp_10=zh_i*16+zh_j;
	return 	temp_10;
}


















