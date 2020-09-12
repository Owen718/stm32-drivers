#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end

//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//#define xcrc(crc,cp) (crctab[((crc>>8) & 0xff)^(cp & 0xff)]^(crc<<8))   /////校验计算公式
#define xcrc(crc,cp) (crctab[((crc&0x00ff) & 0xff)^(cp & 0xff)]^(crc>>8))   /////校验计算公式
#define USART_CS 8			//宏定义串口接收超时

//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记
	  
u16 const crctab[256] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441, 
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040, 
};
u16 GgetCmd_CRC=0;//串口接收到上位机的CRC16校验和
u16 GgetCmd_CRC_INT=0;
u16 GgetCmd_CRC_SUM=0;
u8 GgetCmd_CRC_i=0;
u16   USART1_jsq=0;
u16   USART2_jsq=0;
u16   USART3_jsq=0;
u16	 USART1_R[100]={0};
u16	 USART2_R[100]={0};
u16	 USART3_R[100]={0};
u8 flag_R3=0;		//
u8 flag_R1=0;	

u32 LRC_sum1=0;		//定义的校验和
u16 LRC_jyw1=0;		//定义的校验位
u16 send_i_com2=0;	//LRC校验for循环计数器
u8 LRC_jyw1_b=0,LRC_jyw1_s=0,LRC_jyw1_g=0; 	//LRC校验百十个位
u32 JS_new=0, JS_old=0,JS_cz=0;//定义串口接收中断里面的现在值、过去值、和差值。
u32 JS_new3=0, JS_old3=0,JS_cz3=0;//定义串口接收中断里面的现在值、过去值、和差值。
u32 JS_new2=0, JS_old2=0,JS_cz2=0;//定义串口接收中断里面的现在值、过去值、和差值。
extern u32 TIME3;
u8 flag_wendu=0;
void USART1_IRQHandler(void)
{
	u8 res;	
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR;
		JS_new=TIME3;  			//将定时器3的计数器赋值给新值
		JS_cz=JS_new-JS_old;	 //计算差值
		JS_old=JS_new;			//将新值赋值给旧值
		if(JS_cz>USART_CS)	  		//如果差值大于设定的时间
		{
			 USART1_jsq=0;		   //代表另一串数据到来，计数器从零开始计数
		} 
		USART1_jsq++;
		USART1_R[USART1_jsq] = res;	
		if(USART1_jsq==2) 
		{
			//if(USART1_R[1]==0x54 && USART1_R[2]==0x0a) 
			{
				//JS_new=0;								//现在值清零
				//JS_old=0;	 							//过去值赋值0
				//USART1_jsq=0;
				//flag_wendu=1;
			}
		}	
	}
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif										 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQChannel,2);//组2，最低优先级 
#endif
}
/**********************************************************************************	 
*  串口1发送一个字节                                                              *
**********************************************************************************/	
void USART1_Send(u8 data)
{
    USART1->DR=data;
	while(!(USART1->SR>>7)&0x01);//等待发送结束
}
/*************************************************************************
函数名称: void USART3_Init(void) 
函数说明:串口3中断初始化程序
函数功能:
函数变量:
注    意:
*************************************************************************/
/**********************************************************************************	 
*  初始化IO 串口3                                                                 *
*  pclk1:PCLK1时钟频率(Mhz)                                                       *
*  bound:波特率                                                                   *
*  CHECK OK                                                                       *
*  091209
//串口3初始化程序里禁止接收程序。串口3作为GPRS远程监控口，不禁止接收，上电断电GPRS 
//会影响电路板程序运行，电路板进入了串口3的接收程序。
//接收数据允许，当GPRS断电和上电都会影响电路板的程序进入接收。
                                                                         *
**********************************************************************************/								
void USART3_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;     //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;    //使能串口3时钟 
	GPIOB->CRH&=0XFFFF00FF; //IO状态设置
	GPIOB->CRH|=0X00008B00; //IO状态设置
		  
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa;   // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.	   0000 0000 0000 0000  0010 0000 0000 1100
	//USART3->CR1|=0X208C;    //1位停止,无校验位.	   0000 0000 0000 0000  0010 0000 1000 1100	  发送中断使能
	USART3->CR1|=1<<8;      //PE中断使能   校验错误进入中断
//接收数据允许，当GPRS断电和上电都会影响电路板的程序进入接收
	USART3->CR1|=1<<5;      //接收缓冲区非空中断使能      这句禁止接收串口中断数据  20151231-	    	
	MY_NVIC_Init(3,0,USART3_IRQChannel,2);//组2，最低优先级   1,2
}
/*************************************************************************
函数名称: void USART3_IRQHandler(void) 
函数说明:串口3中断程序
函数功能:
函数变量:
注    意:
*************************************************************************/
/**********************************************************************************	 
*                           串口3中断接收函数                                     *
**********************************************************************************/
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
void USART3_IRQHandler(void)
{
	u8 res3;
	 if(USART3->SR&(1<<5))//接收到数据
	{	 
	  res3=USART3->DR; 
	  JS_new3=TIME3;  			//将定时器3的计数器赋值给新值
	  JS_cz3=JS_new3-JS_old3;	 //计算差值
	  JS_old3=JS_new3;			//将新值赋值给旧值
	  if(JS_cz3>USART_CS)	  		//如果差值大于设定的时间
		{
			 USART3_jsq=0;		   //代表另一串数据到来，计数器从零开始计数
		} 
	  USART3_jsq++;
	  USART3_R[USART3_jsq]=res3;  	
	  if(USART3_R[1]==0x01 && USART3_R[2]==0x03 && USART3_R[3]==0x04 && USART3_jsq==9)
		{
			JS_new3=0;								//现在值清零
			JS_old3=0;	 							//过去值赋值0
			GgetCmd_CRC=USART3_R[USART3_jsq]*256+USART3_R[USART3_jsq-1];//计算接收到的CRC校验和
			GgetCmd_CRC_INT=0xffff;
			GgetCmd_CRC_SUM=0;
			for(GgetCmd_CRC_i=1;GgetCmd_CRC_i<(USART3_jsq-1);GgetCmd_CRC_i++)
			{
				GgetCmd_CRC_SUM=xcrc(GgetCmd_CRC_INT,USART3_R[GgetCmd_CRC_i]);
				GgetCmd_CRC_INT=GgetCmd_CRC_SUM;
			}
			USART3_jsq=0;

			if(GgetCmd_CRC==GgetCmd_CRC_SUM)
			{
				flag_R3=1;	  		//接收成功标志
			}
		} 									     
	} 
} 
/**********************************************************************************	 
*  串口3中断发送结束                                                              *
**********************************************************************************/	
void USART3_Send(u8 data)
{
    USART3->DR=data;
	while(!(USART3->SR>>7)&0x01);//μè′y·￠?í?áê?
}

/*************************************************************************
函数名称: void USART2_Init(void) 
函数说明:串口2中断初始化程序
函数功能:
函数变量:
注    意:
*************************************************************************/
/**********************************************************************************	 
*  初始化IO 串口2                                                                 *
*  pclk2:PCLK1时钟频率(Mhz)                                                       *
*  bound:波特率                                                                   *
*  CHECK OK                                                                       *
*  091209                                                                         *
**********************************************************************************/								
void USART2_Init(u32 U2_pclk1,u32 U2_bound)
{  	 
	float U2_temp;
	u16 U2_mantissa;
	u16 U2_fraction;	   
	U2_temp=(float)(U2_pclk1*1000000)/(U2_bound*16);//得到USARTDIV
	U2_mantissa=U2_temp;				 //得到整数部分
	U2_fraction=(U2_temp-U2_mantissa)*16; //得到小数部分	 
    U2_mantissa<<=4;
	U2_mantissa+=U2_fraction; 
	RCC->APB2ENR|=1<<2;     //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;    //使能串口2时钟 
	GPIOA->CRL&=0XFFFF00FF; //IO状态设置
	GPIOA->CRL|=0X00008B00; //IO状态设置
	//GPIOA->CRL=0X44448B44; //IO状态设置	 
	 
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=U2_mantissa;   // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.	   0000 0000 0000 0000  0010 0000 0000 1100
	//USART2->CR1|=0X208C;    //1位停止,无校验位.	   0000 0000 0000 0000  0010 0000 1000 1100	  发送中断使能
	USART2->CR1|=1<<8;      //PE中断使能   校验错误进入中断
	USART2->CR1|=1<<5;      //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1,1,USART2_IRQChannel,2);//抢占优先级 相应优先级 通道 组2，最低优先级  
}
/*************************************************************************
函数名称: void USART2_IRQHandler(void) 
函数说明:串口2中断程序
函数功能:
函数变量:
注    意:
*************************************************************************/
/**********************************************************************************	 
*                           串口2中断接收函数                                     *
**********************************************************************************/
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
void USART2_IRQHandler(void)	
{	
	u8 res2;	
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res2=USART2->DR;
		JS_new2=TIME3;  			//将定时器3的计数器赋值给新值
		JS_cz2=JS_new2-JS_old2;	 //计算差值
		JS_old2=JS_new2;			//将新值赋值给旧值
		if(JS_cz2>USART_CS)	  		//如果差值大于设定的时间
		{
			 USART2_jsq=0;		   //代表另一串数据到来，计数器从零开始计数
		} 
		USART2_jsq++;
		USART2_R[USART2_jsq] = res2;	
		//USART1_Send(USART2_R[USART2_jsq]);
		//if(res2==0x3a)		//如果检测到第一个字符为：则将计数器置1  开始记录数据
		//{
		//	USART2_R[1] = res2;
		//	USART2_jsq=1;
		//	flag_CLJG=0;		//测量结果标志置0
		//}
	}									     	    
} 


/**********************************************************************************	 
*  串口2发送一个字节                                                               *
**********************************************************************************/	
void USART2_Send(u8 data)
{
    USART2->DR=data;
	while(!(USART2->SR>>7)&0x01);//等待发送结束
}



