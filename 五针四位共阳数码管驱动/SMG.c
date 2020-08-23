#include "SMG.h"
#include "stm32f10x.h"                  // Device header
#include "delay.h"

/*
const unsigned char LED_table[24]={
//01234567
0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
//89ABCDEF
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,
//时钟效果
0xF9,0xBF,0x99,0xBF,0x82,0xF9,0xC0,0xA4
};

//共阳数码管段码“0123456789AbCdEF熄灭-”
const unsigned char wei_table[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//第一位至第八位位码
*/


/*使用范例：
//四位共阳数码管，2个74HC595联极
SMG_Display(LED_table[0],wei_table[1]); 第1位显示数据0 
SMG_Display(LED_table[1] & 0x7f,wei_table[2]);  第2位显示数据 1.  
SMG_Display(LED_table[2],wei_table[3]);   第3位显示数据 2
*/





void HC595_Init()  //初始化函数
{
		
	GPIO_InitTypeDef IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //GPIOC 引脚初始化
	IO.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	IO.GPIO_Speed = GPIO_Speed_50MHz;  
	IO.GPIO_Pin = DIO|SCLK|RCLK;
	GPIO_Init(GPIOC,&IO);

}

void HC595_WR(unsigned char data)
{
	u8 i;
	for(i = 0;i<8;i++)
	{
		if(data&0x80)//逐一取最高位
			SET_DIO;//写1
		else
			CLR_DIO;//写0
		
			data<<=1;//将次高位置为最高位
			CLR_SCLK;  //时钟上升沿 0 
			SET_SCLK;//时钟上升沿   1
	}
}

void HC595_Over()//控制RCLK时钟上升沿
{

		CLR_RCLK;   //0
		SET_RCLK;   //1
		
}



void SMG_Display(unsigned char data,unsigned char wei)  //显示函数 data是数码管显示数据，wei是哪一位
{

	HC595_WR(data); //送出段码
	HC595_WR(wei);//送出位码
	HC595_Over(); //输出数据
	delay_ms(2);

}

