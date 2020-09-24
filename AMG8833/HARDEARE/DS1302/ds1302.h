#ifndef __DS1302_H
#define __DS1302_H
//#include "sys.h"



#define DS1302_SCK PBout(9)
#define DS1302_CE PBout(7)

#define DS1302_I PBin(8)
#define DS1302_O PBout(8)

#define DS1302IO_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0x00000008;}
#define DS1302IO_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0x00000003;}



//寄存器宏定义
#define WRITE_SECOND  0x80
#define WRITE_MINUTE  0x82
#define WRITE_HOUR    0x84
#define WRITE_DAY     0x86
#define WRITE_MONTH   0x88
#define WRITE_WEEK    0x8A
#define WRITE_YEAR    0x8C


#define READ_SECOND   0x81
#define READ_MINUTE   0x83
#define READ_HOUR     0x85
#define READ_DAY      0x87
#define READ_MONTH    0x89
#define READ_WEEK     0x8B
#define READ_YEAER    0x8D


#define WRITE_PROTECT 0x8E


#define WRITE1302_RAM01  0xC0
#define WRITE1302_RAM02  0xC2
#define WRITE1302_RAM03  0xC4
#define WRITE1302_RAM04  0xC6
#define WRITE1302_RAM05  0xC8

#define READ1302_RAM01   0xC1
#define READ1302_RAM02   0xC3
#define READ1302_RAM03   0xC5
#define READ1302_RAM04   0xC7
#define READ1302_RAM05   0xC9

//extern u8 time1302_year;				           /*年*/
//extern u8 time1302_month;						     /*月*/
//extern u8 time1302_day;							     /*日*/
//extern u8 time1302_hour;							     /*时*/
//extern u8 time1302_minute;						     /*分*/
//extern u8 time1302_second;						     /*秒*/

//extern u8 DS1302_time[10];
extern u8  year_byte; //保存年的后两位
extern u8 ds1302_xz;//20180817+串口接收到上位机同步时间了，将DS1302的初始时间更新
extern u32 ds1302_jsq;//20180817+主程序读取ds1302的时间次数，目前是循环1000次，读取一次时间信息


void DS1302IO_Init(void);
void DS1302_TimeInit(void);
void Write1302 (u8 addr,u8 dat);
u8 Read1302(u8 addr);


void read_ds1302time(void);

u8 zh10to_BCD(u8 temp_16);

#endif







