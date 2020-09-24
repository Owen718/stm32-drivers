#include "moto.h"



//***************************���õ������IO��***************************//
//�������̼����������ķ�ֵ���
//Ӳ������˵����

//ǰ���������L298N
//PC6---ENA������С����ǰ����ٶ�
//PC7---ENB������С����ǰ����ٶ�
//PC0---IN1��PC1---IN2 ����С����ǰ�������ת
//PC2---IN3��PC3---IN4 ����С����ǰ�������ת

//�󲿵������L298N
//PC9---ENB������С��������ٶ�
//PC8---ENA������С���Һ����ٶ�
//PC10---IN1 ��PC11---IN2  ����С���Һ���������ת
//PC12---IN3�� PD2---IN4   ����С�������������ת


void MOTO_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		//ǰ���������
		RCC_APB2PeriphClockCmd(MOTO_Q_CLK, ENABLE);                                	  /*����GPIO������ʱ��*/																   
		GPIO_InitStructure.GPIO_Pin = MOTO_ZQ1 | MOTO_ZQ2	| MOTO_YQ1 | MOTO_YQ2;	/*ѡ��Ҫ���Ƶ�GPIO����*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*��������ģʽΪͨ���������*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*������������Ϊ50MHz */ 
		GPIO_Init(MOTO_Q_PORT, &GPIO_InitStructure); 	/*���ÿ⺯������ʼ��GPIO*/	
	
		//MOTO_ZH2 ��PD2
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);                                	  /*����GPIO������ʱ��*/																   
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;	/*ѡ��Ҫ���Ƶ�GPIO����*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*��������ģʽΪͨ���������*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*������������Ϊ50MHz */ 
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
		//���������
		RCC_APB2PeriphClockCmd( MOTO_H_CLK, ENABLE);                                	  /*����GPIO������ʱ��*/																   
		GPIO_InitStructure.GPIO_Pin = MOTO_ZH1 | MOTO_YH2 | MOTO_YH1;	/*ѡ��Ҫ���Ƶ�GPIO����*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*��������ģʽΪͨ���������*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*������������Ϊ50MHz */ 
		GPIO_Init(MOTO_H_PORT, &GPIO_InitStructure);                                    /*���ÿ⺯������ʼ��GPIO*/	
	
	
}
//��ǰ���
void MOTO_ZQ(char state)
{
	if(state == GO)//��ǰ���ǰ��
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_ZQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ2);
	}
	if(state == BACK)//��ǰ�������
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_ZQ2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ1);
		
	}
	if(state == STOP)//ͣת
	{
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ2);
	}
		
		
}

//��ǰ���
void MOTO_YQ(char state)
{
	if(state == GO)//��ǰ���ǰ��
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_YQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ2);
	}
	if(state == BACK)//��ǰ�������
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_YQ2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ1);
	
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ2);
	}
}

//�����
void MOTO_ZH(char state)
{
	if(state == GO)//�����ǰ��
	{
		GPIO_SetBits(GPIOD,MOTO_ZH2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZH1);

	}
	if(state == BACK)//���������
	{

		GPIO_SetBits(MOTO_Q_PORT,MOTO_ZH1);
	  GPIO_ResetBits(GPIOD,MOTO_ZH2);
	
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZH1);
	  GPIO_ResetBits(GPIOD,MOTO_ZH2);
	}
		
}

//�Һ���
void MOTO_YH(char state)
{
	if(state == GO)//�Һ���ǰ��
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_YH2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH1);
		
	}
	if(state == BACK)//�Һ�������
	{

		GPIO_SetBits(MOTO_Q_PORT,MOTO_YH1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH2);
	}
	if(state == STOP)//ͣת
	{
		GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH2);
	}
		
}


//***************************ǰ��***************************//
//ֻҪ����INx()��״̬�Ϳ��Ըı���ת������
void Car_Go(void)
{
	//��ǰ��� ǰ    //��ǰ��� ǰ
	MOTO_ZQ(GO);       MOTO_YQ(GO);

	//����� ǰ   //�Һ��� ǰ
	MOTO_ZH(GO);       MOTO_YH(GO);
	
	
}

void Car_Back(void)
{
		//��ǰ��� ��    //��ǰ��� ��
MOTO_ZQ(BACK);       MOTO_YQ(BACK);

	//����� ��   //�Һ��� ��
MOTO_ZH(BACK);       MOTO_YH(BACK);
}

//***************************����תȦ***************************//
void Car_Turn_Left(void)
{
	
	//��ǰ��� ��    //��ǰ��� ǰ
MOTO_ZQ(BACK);       MOTO_YQ(GO);

	//����� ��   //�Һ��� ǰ
MOTO_ZH(BACK);       MOTO_YH(GO);
	
}


//***************************����תȦ***************************//
void Car_Turn_Right(void)
{
	//��ǰ��� ǰ    //��ǰ��� ��
MOTO_ZQ(GO);       MOTO_YQ(BACK);

	//����� ǰ   //�Һ��� ��
  MOTO_ZH(GO);       MOTO_YH(BACK);
	
}
//**************************����ƽ��************************//
void Car_GO_Right(void)
{
//��ǰ��� ��      //��ǰ��� ǰ
MOTO_ZQ(BACK);    MOTO_YQ(GO);
//�����  ǰ     //�Һ��� ��
MOTO_ZH(GO);      MOTO_YH(BACK);

}

//**************************����ƽ��************************//
void Car_GO_Left(void)
{
//��ǰ��� ǰ      //��ǰ��� ��
MOTO_ZQ(GO);    MOTO_YQ(BACK);
//�����  ��     //�Һ��� ǰ
MOTO_ZH(BACK);      MOTO_YH(GO);

}



//***************************ͣ��***************************//
void Car_Stop(void)
{
	//��ǰ��� ͣ    //��ǰ��� ͣ
MOTO_ZQ(STOP);       MOTO_YQ(STOP);

	//����� ͣ   //�Һ��� ͣ
  MOTO_ZH(STOP);       MOTO_YH(STOP);
}



