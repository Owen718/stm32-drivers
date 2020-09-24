#include "MLX90614.h"
#include "delay.h"

/*  MLX90614 驱动代码  *
****************************
*	auchor:yetian
*	time:2020/9/23
*****************************/

#define ACK	 0 //应答
#define	NACK 1 //无应答
#define SA				0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM存取命令
#define RAM_TOBJ1		0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度

#define SMBUS_PORT	GPIOB      //PB端口(端口和下面的两个针脚可自定义)
#define SMBUS_SCL		GPIO_Pin_8 //PB6：SCL
#define SMBUS_SDA		GPIO_Pin_9 //PB7：SDA

#define RCC_APB2Periph_SMBUS_PORT		RCC_APB2Periph_GPIOB

#define SMBUS_SCL_H()	    SMBUS_PORT->BSRR = SMBUS_SCL //置高电平
#define SMBUS_SCL_L()	    SMBUS_PORT->BRR = SMBUS_SCL  //置低电平
#define SMBUS_SDA_H()	    SMBUS_PORT->BSRR = SMBUS_SDA
#define SMBUS_SDA_L()	    SMBUS_PORT->BRR = SMBUS_SDA

#define SMBUS_SDA_PIN()	    SMBUS_PORT->IDR & SMBUS_SDA //读取引脚电平



/* 起始信号 函数	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: 输出起始信号
*	parameter:void
*	intention:使用前需要先初始化delay
*****************************/
void SMBus_StartBit(void)
{
	
	SMBUS_SDA_H();  //拉高数据线
	SMBus_Delay(5);  //延时几微秒
	SMBUS_SCL_H();  //拉高时钟线
	SMBus_Delay(5);  //延时几微秒
	SMBUS_SDA_L();  //拉低数据线
	SMBus_Delay(5);  //延时几微秒
	SMBUS_SCL_L();  //拉低时钟线
	SMBus_Delay(5); //延时几微秒
}

/* 终止信号 函数	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: 输出终止信号
*	parameter:void
*	attention:使用前需要先初始化delay
*****************************/
void SMBus_StopBit(void)
{
	
	SMBUS_SCL_L();  //拉低时钟线
	SMBus_Delay(5);  //延时几微秒
	SMBUS_SDA_L();  //拉低时钟线
	SMBus_Delay(5);  //延时几微秒
	SMBUS_SCL_H();  //拉高数据线
	SMBus_Delay(5);  //延时几微秒
	SMBUS_SDA_H();  //拉高时钟线

}


/*  发送字节函数	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: 发出一个字节
*	parameter:u8 Tx_buffer
*****************************/


u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;
    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)//如果最高位为1
        {
            bit_out=1;   // 把最高位置1
        }
        else  //如果最高位为0
        {
            bit_out=0;  // 把最高位置0
        }
        SMBus_SendBit(bit_out);	// 把最高位发送出去
        Tx_buffer<<=1;// 左移一位把最高位移出去等待下一个最高位，循环8次，每次都发最高位，就可把一个字节发出去了
    }
    Ack_bit=SMBus_ReceiveBit();	// Get acknowledgment bit
    return	Ack_bit;
}


/*  发送一比特函数	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: 发出一个bit
*	parameter:u8 Bit_out
*****************************/
 
void SMBus_SendBit(u8 Bit_out)
{
	if(Bit_out == 0) 
		SMBUS_SDA_L(); //0为低
	else
		SMBUS_SDA_H(); //1为高

	SMBus_Delay(2);
	SMBUS_SCL_H();
	SMBus_Delay(6);
	SMBUS_SCL_L();
	SMBus_Delay(3);
	
	return; 
}

/*  接收一比特函数	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: 接收一个bit
*	parameter:void
*	return : u8 Ack_bit
*****************************/

u8 SMBus_ReceiveBit(void)
{
	u8 Ack_bit;
	
	SMBUS_SDA_H(); //引脚靠外部电阻上拉，作为输入
	SMBus_Delay(2);  
	SMBUS_SCL_H();  //s设置时钟线
	SMBus_Delay(5); //高时钟脉冲
	if(SMBUS_SDA_PIN())
		Ack_bit = 1;
	else
		Ack_bit = 0;
	SMBUS_SCL_L();  //拉低时钟线
	SMBus_Delay(3);
	
	return Ack_bit; 
}



/*  接收字节函数	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: 接收一个字节
*	parameter:u8 Tx_buffer
*****************************/

u8 SMBus_ReceiveByte(u8 ack_nack)
{
	u8 RX_buffer;
	u8 Bit_Counter;
	
	for(Bit_Counter = 8;Bit_Counter;Bit_Counter--)
	{
		if(SMBus_ReceiveBit())  //从数据线中获取一比特信息
		{
			RX_buffer<<=1;  //移位
			RX_buffer |=0x01; //保存数值 1
		}
		else
		{
			RX_buffer <<= 1;  //移位
			RX_buffer &= 0xfe; //保存数值0
		}
	}
	SMBus_SendBit(ack_nack);
	return RX_buffer;
}

/*******************************************************************************
* Function Name  : SMBus_Delay
* Description    : 延时  一次循环约1us
* Input          : time
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Delay(u16 time)
{
    u16 i, j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<time; j++);
    }
}






/* MLX90614初始化函数  *
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: MLX90614初始化
*	parameter:void
*****************************/

void SMBus_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SMBUS_PORT,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SMBUS_SCL | SMBUS_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SMBUS_PORT,&GPIO_InitStructure);

	SMBUS_SCL_H();
	SMBUS_SDA_H();
	
}



/*******************************************************************************
 * Function Name  : SMBus_ReadMemory
 * Description    : READ DATA FROM RAM/EEPROM
 * Input          : slaveAddress, command
 * Output         : None
 * Return         : Data
*******************************************************************************/
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;			// Data storage (DataH:DataL)
    u8 Pec;				// PEC byte storage
    u8 DataL=0;			// Low data byte storage
    u8 DataH=0;			// High data byte storage
    u8 arr[6];			// Buffer for the sent bytes
    u8 PecReg;			// Calculated PEC byte storage
    u8 ErrorCounter;	// Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;				// Initialising of ErrorCounter
	slaveAddress <<= 1;	//2-7位表示从机地址
	
    do
    {
repeat:
        SMBus_StopBit();			    //If slave send NACK stop comunication
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //Yes,go out from do-while{}
        }

        SMBus_StartBit();				//Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress 最低位Wr=0表示接下来写命令
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again
        }

        SMBus_StartBit();					//Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress 最低位Rd=1表示接下来读数据
        {
            goto	repeat;             	//Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK);	//Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK); //Read high data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);	//Read PEC byte, master must send NACK
        SMBus_StopBit();				//Stop condition

        arr[5] = slaveAddress;		//
        arr[4] = command;			//
        arr[3] = slaveAddress+1;	//Load array arr
        arr[2] = DataL;				//
        arr[1] = DataH;				//
        arr[0] = 0;					//
        PecReg=PEC_Calculation(arr);//Calculate CRC
    }
    while(PecReg != Pec);		//If received and calculated CRC are equal go out from do-while{}

	data = (DataH<<8) | DataL;	//data=DataH:DataL
    return data;
}

/*******************************************************************************
* Function Name  : PEC_calculation
* Description    : Calculates the PEC of received bytes
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];
    u8	BitPosition=47;
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

 /*******************************************************************************
 * Function Name  : SMBus_ReadTemp
 * Description    : Calculate and return the temperature
 * Input          : None
 * Output         : None
 * Return         : SMBus_ReadMemory(0x00, 0x07)*0.02-273.15
*******************************************************************************/
float SMBus_ReadTemp(void)
{   
	float temp;
	temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
	return temp;
}

/*********************************END OF FILE*********************************/





