#include "DS1302.h"
#include "delay.h"
#include "usart.h"
/*
DS1302接口:
	GPIOA_5 ->DS1302_RST
	GPIOA_8 ->DS1302_DAT
	GPIOA_7 ->DS1302_CLK
*/
struct TIMEData TimeData;
//DS1302地址定义
#define DS1302_SEC_ADDR           0x80		//秒数据地址
#define DS1302_MIN_ADDR           0x82		//分数据地址
#define DS1302_HOUR_ADDR          0x84		//时数据地址
#define DS1302_DAY_ADDR           0x86		//日数据地址
#define DS1302_MONTH_ADDR         0x88		//月数据地址
#define DS1302_WEEK_ADDR          0x8a		//星期数据地址
#define DS1302_YEAR_ADDR          0x8c		//年数据地址
#define DS1302_CONTROL_ADDR       0x8e		//控制数据地址
#define DS1302_CHARGER_ADDR       0x90 		//充电功能地址			 
#define DS1302_CLKBURST_ADDR      0xbe
 
//初始时间定义
u8 time_buf[8] = {0x20,0x19,0x04,0x12,0x15,0x07,0x00,0x06};//初始时间2019年4月12号15点07分00秒 星期五
u8 readtime[15];//当前时间
u8 sec_buf=0;  //秒缓存
u8 sec_flag=0; //秒标志位
 
//DS1302初始化函数
void DS1302_Init() 
{
	/*1.GPIOA时钟*/
	RCC->APB2ENR |= 1<<2;
	/*2. 配置GPIOA_5/6/7模式*/
	GPIOA->CRL &= 0X000FFFFF;
	GPIOA->CRL |= 0X33300000;
	GPIOA->ODR |=1<<8;
	//printf("DS1302_Init OK!\n");
 
}
//向DS1302写入一字节数据
void DS1302_WriteByte(u8 addr,u8 data) 
{
	u8 i;
	DS1302_RST=0; //禁止数据传输 ！！！这条很重要
	DS1302_CLK=0; //确保写数据前SCLK为低电平
	DS1302_RST=1;	//启动DS1302总线	
	DS1302_OutPut_Mode();
	addr=addr&0xFE;  //最低位置零，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) //写入目标地址：addr
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //时钟上升沿写入数据
		DS1302_CLK=0;
		addr=addr>>1;
	}	
	for (i=0;i<8;i++) //写入数据：data
	{
		if(data&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1;    //时钟上升沿写入数据
		DS1302_CLK=0;
		data = data >> 1;
	}
	DS1302_CLK=1;    // 将时钟电平置于高电平状态 ，处于已知状态
	DS1302_RST=0;	//停止DS1302总线
}
 
//从DS1302读出一字节数据
u8 DS1302_ReadByte(u8 addr) 
{
	u8 i,temp;	
	DS1302_RST=0; //这条很重要
	DS1302_CLK=0; //先将SCLK置低电平,确保写数居前SCLK被拉低
	DS1302_RST=1; //启动DS1302总线
	DS1302_OutPut_Mode();
	//写入目标地址：addr
	addr=addr|0x01; //最低位置高，寄存器0位为0时写，为1时读
	for(i=0;i<8;i++) 
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //写数据
		DS1302_CLK=0;
		addr = addr >> 1;
	}	
	//从DS1302读出数据：temp
	DS1302_InPut_Mode();
	for(i=0;i<8;i++)
	{
		temp=temp>>1;
		if (DS1302_IN) temp|=0x80;
		else temp&=0x7F;
		DS1302_CLK=1;
		DS1302_CLK=0;
	}	
	DS1302_CLK=1;  //将时钟电平置于已知状态
	DS1302_RST=0;	//停止DS1302总线
	return temp;
}
//向DS1302写入时钟数据,用于时间校准修改
void DS1302_WriteTime() 
{
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x00);       //关闭写保护 
	DS1302_WriteByte(DS1302_SEC_ADDR,0x80);           //暂停时钟 
	//DS1302_WriteByte(DS1302_CHARGER_ADDR,0xa9);     //涓流充电 
	DS1302_WriteByte(DS1302_YEAR_ADDR,time_buf[1]);   //年 
	DS1302_WriteByte(DS1302_MONTH_ADDR,time_buf[2]);  //月 
	DS1302_WriteByte(DS1302_DAY_ADDR,time_buf[3]);    //日 
	DS1302_WriteByte(DS1302_HOUR_ADDR,time_buf[4]);   //时 
	DS1302_WriteByte(DS1302_MIN_ADDR,time_buf[5]);    //分
	DS1302_WriteByte(DS1302_SEC_ADDR,time_buf[6]);    //秒
	DS1302_WriteByte(DS1302_WEEK_ADDR,time_buf[7]);	  //周 
	DS1302_WriteByte(DS1302_CHARGER_ADDR,0xA5);//打开充电功能 选择2K电阻充电方式
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x80);//打开写保护     
}
//从DS1302读出时钟数据
void DS1302_ReadTime(void)  
{
	time_buf[1]=DS1302_ReadByte(DS1302_YEAR_ADDR);          //年 
	time_buf[2]=DS1302_ReadByte(DS1302_MONTH_ADDR);         //月 
	time_buf[3]=DS1302_ReadByte(DS1302_DAY_ADDR);           //日 
	time_buf[4]=DS1302_ReadByte(DS1302_HOUR_ADDR);          //时 
	time_buf[5]=DS1302_ReadByte(DS1302_MIN_ADDR);           //分 
	time_buf[6]=(DS1302_ReadByte(DS1302_SEC_ADDR))&0x7f;    //秒，屏蔽秒的第7位，避免超出59
	time_buf[7]=DS1302_ReadByte(DS1302_WEEK_ADDR);          //周 	
}
 
//主函数
void DS1302_GetTime()
{ 
    DS1302_ReadTime(); //读取时间
    TimeData.year=(time_buf[0]>>4)*1000+(time_buf[0]&0x0F)*100+(time_buf[1]>>4)*10+(time_buf[1]&0x0F); //计算年份
    TimeData.month=(time_buf[2]>>4)*10+(time_buf[2]&0x0F);  //计算月份
    TimeData.day=(time_buf[3]>>4)*10+(time_buf[3]&0x0F);    //计算日期
    TimeData.hour=(time_buf[4]>>4)*10+(time_buf[4]&0x0F);   //计算小时
    TimeData.minute=(time_buf[5]>>4)*10+(time_buf[5]&0x0F); //计算分钟
    TimeData.second=(time_buf[6]>>4)*10+(time_buf[6]&0x0F); //计算秒钟
    TimeData.week=(time_buf[7]&0x0F);                       //计算星期
  //printf("时间:%d-%d-%d %d:%d:%d %d \n",TimeData.year,TimeData.month,TimeData.day,TimeData.hour,TimeData.minute,TimeData.second,TimeData.week);																												
}
