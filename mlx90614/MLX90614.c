#include "MLX90614.h"
#include "delay.h"

/*  MLX90614 ��������  *
****************************
*	auchor:yetian
*	time:2020/9/23
*****************************/

#define ACK	 0 //Ӧ��
#define	NACK 1 //��Ӧ��
#define SA				0x00 //Slave address ����MLX90614ʱ��ַΪ0x00,���ʱ��ַĬ��Ϊ0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM��ȡ����
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM��ȡ����
#define RAM_TOBJ1		0x07 //To1 address in the eeprom Ŀ��1�¶�,��⵽�ĺ����¶� -70.01 ~ 382.19��

#define SMBUS_PORT	GPIOB      //PB�˿�(�˿ں������������ſ��Զ���)
#define SMBUS_SCL		GPIO_Pin_8 //PB6��SCL
#define SMBUS_SDA		GPIO_Pin_9 //PB7��SDA

#define RCC_APB2Periph_SMBUS_PORT		RCC_APB2Periph_GPIOB

#define SMBUS_SCL_H()	    SMBUS_PORT->BSRR = SMBUS_SCL //�øߵ�ƽ
#define SMBUS_SCL_L()	    SMBUS_PORT->BRR = SMBUS_SCL  //�õ͵�ƽ
#define SMBUS_SDA_H()	    SMBUS_PORT->BSRR = SMBUS_SDA
#define SMBUS_SDA_L()	    SMBUS_PORT->BRR = SMBUS_SDA

#define SMBUS_SDA_PIN()	    SMBUS_PORT->IDR & SMBUS_SDA //��ȡ���ŵ�ƽ



/* ��ʼ�ź� ����	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: �����ʼ�ź�
*	parameter:void
*	intention:ʹ��ǰ��Ҫ�ȳ�ʼ��delay
*****************************/
void SMBus_StartBit(void)
{
	
	SMBUS_SDA_H();  //����������
	SMBus_Delay(5);  //��ʱ��΢��
	SMBUS_SCL_H();  //����ʱ����
	SMBus_Delay(5);  //��ʱ��΢��
	SMBUS_SDA_L();  //����������
	SMBus_Delay(5);  //��ʱ��΢��
	SMBUS_SCL_L();  //����ʱ����
	SMBus_Delay(5); //��ʱ��΢��
}

/* ��ֹ�ź� ����	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: �����ֹ�ź�
*	parameter:void
*	attention:ʹ��ǰ��Ҫ�ȳ�ʼ��delay
*****************************/
void SMBus_StopBit(void)
{
	
	SMBUS_SCL_L();  //����ʱ����
	SMBus_Delay(5);  //��ʱ��΢��
	SMBUS_SDA_L();  //����ʱ����
	SMBus_Delay(5);  //��ʱ��΢��
	SMBUS_SCL_H();  //����������
	SMBus_Delay(5);  //��ʱ��΢��
	SMBUS_SDA_H();  //����ʱ����

}


/*  �����ֽں���	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: ����һ���ֽ�
*	parameter:u8 Tx_buffer
*****************************/


u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;
    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)//������λΪ1
        {
            bit_out=1;   // �����λ��1
        }
        else  //������λΪ0
        {
            bit_out=0;  // �����λ��0
        }
        SMBus_SendBit(bit_out);	// �����λ���ͳ�ȥ
        Tx_buffer<<=1;// ����һλ�����λ�Ƴ�ȥ�ȴ���һ�����λ��ѭ��8�Σ�ÿ�ζ������λ���Ϳɰ�һ���ֽڷ���ȥ��
    }
    Ack_bit=SMBus_ReceiveBit();	// Get acknowledgment bit
    return	Ack_bit;
}


/*  ����һ���غ���	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: ����һ��bit
*	parameter:u8 Bit_out
*****************************/
 
void SMBus_SendBit(u8 Bit_out)
{
	if(Bit_out == 0) 
		SMBUS_SDA_L(); //0Ϊ��
	else
		SMBUS_SDA_H(); //1Ϊ��

	SMBus_Delay(2);
	SMBUS_SCL_H();
	SMBus_Delay(6);
	SMBUS_SCL_L();
	SMBus_Delay(3);
	
	return; 
}

/*  ����һ���غ���	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: ����һ��bit
*	parameter:void
*	return : u8 Ack_bit
*****************************/

u8 SMBus_ReceiveBit(void)
{
	u8 Ack_bit;
	
	SMBUS_SDA_H(); //���ſ��ⲿ������������Ϊ����
	SMBus_Delay(2);  
	SMBUS_SCL_H();  //s����ʱ����
	SMBus_Delay(5); //��ʱ������
	if(SMBUS_SDA_PIN())
		Ack_bit = 1;
	else
		Ack_bit = 0;
	SMBUS_SCL_L();  //����ʱ����
	SMBus_Delay(3);
	
	return Ack_bit; 
}



/*  �����ֽں���	*
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: ����һ���ֽ�
*	parameter:u8 Tx_buffer
*****************************/

u8 SMBus_ReceiveByte(u8 ack_nack)
{
	u8 RX_buffer;
	u8 Bit_Counter;
	
	for(Bit_Counter = 8;Bit_Counter;Bit_Counter--)
	{
		if(SMBus_ReceiveBit())  //���������л�ȡһ������Ϣ
		{
			RX_buffer<<=1;  //��λ
			RX_buffer |=0x01; //������ֵ 1
		}
		else
		{
			RX_buffer <<= 1;  //��λ
			RX_buffer &= 0xfe; //������ֵ0
		}
	}
	SMBus_SendBit(ack_nack);
	return RX_buffer;
}

/*******************************************************************************
* Function Name  : SMBus_Delay
* Description    : ��ʱ  һ��ѭ��Լ1us
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






/* MLX90614��ʼ������  *
****************************
*	auchor:yetian
*	time:2020/9/23
*	function: MLX90614��ʼ��
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
	slaveAddress <<= 1;	//2-7λ��ʾ�ӻ���ַ
	
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
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress ���λWr=0��ʾ������д����
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again
        }

        SMBus_StartBit();					//Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress ���λRd=1��ʾ������������
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





