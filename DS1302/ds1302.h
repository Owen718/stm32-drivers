#ifndef DS18B20_H
#define DS18B20_H
#include "stm32f10x.h"
#include "sys.h"
//DS1302引脚定义,可根据实际情况自行修改端口定义
#define DS1302_OutPut_Mode() {GPIOA->CRL &= 0xF0FFFFFF;GPIOA->CRL |= 0x03000000;}
#define DS1302_InPut_Mode()  {GPIOA->CRL &= 0xF0FFFFFF;GPIOA->CRL |= 0x08000000;}
 
#define DS1302_IN  PAin(6)
#define DS1302_OUT PAout(6)
#define DS1302_RST PAout(5)
#define DS1302_CLK PAout(7)
 
struct TIMEData
{
	u16 year;
	u8  month;
	u8  day;
	u8  hour;
	u8  minute;
	u8  second;
	u8  week;
};
extern struct TIMEData TimeData;
extern u8 readtime[15];
void DS1302_Init(void);
void DS1302_WriteByte(u8 addr,u8 data);
u8   DS1302_ReadByte(u8 addr);
void DS1302_WriteTime(void);
void DS1302_ReadTime(void);
void DS1302_GetTime(void);
 
#endif
