#include "moto.h"



//***************************配置电机驱动IO口***************************//
//四驱底盘及四轮麦克纳姆轮底盘
//硬件连接说明：

//前部电机驱动L298N
//PC6---ENA，控制小车左前电机速度
//PC7---ENB，控制小车右前电机速度
//PC0---IN1，PC1---IN2 控制小车左前电机正反转
//PC2---IN3，PC3---IN4 控制小车右前电机正反转

//后部电机驱动L298N
//PC9---ENB，控制小车左后电机速度
//PC8---ENA，控制小车右后电机速度
//PC10---IN1 ，PC11---IN2  控制小车右后电机的正反转
//PC12---IN3， PD2---IN4   控制小车左后电机的正反转


void MOTO_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		//前部两个电机
		RCC_APB2PeriphClockCmd(MOTO_Q_CLK, ENABLE);                                	  /*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin = MOTO_ZQ1 | MOTO_ZQ2	| MOTO_YQ1 | MOTO_YQ2;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
		GPIO_Init(MOTO_Q_PORT, &GPIO_InitStructure); 	/*调用库函数，初始化GPIO*/	
	
		//MOTO_ZH2 在PD2
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);                                	  /*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
		//后部两个电机
		RCC_APB2PeriphClockCmd( MOTO_H_CLK, ENABLE);                                	  /*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin = MOTO_ZH1 | MOTO_YH2 | MOTO_YH1;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
		GPIO_Init(MOTO_H_PORT, &GPIO_InitStructure);                                    /*调用库函数，初始化GPIO*/	
	
	
}
//左前电机
void MOTO_ZQ(char state)
{
	if(state == GO)//左前电机前进
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_ZQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ2);
	}
	if(state == BACK)//左前电机后退
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_ZQ2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ1);
		
	}
	if(state == STOP)//停转
	{
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZQ2);
	}
		
		
}

//右前电机
void MOTO_YQ(char state)
{
	if(state == GO)//右前电机前进
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_YQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ2);
	}
	if(state == BACK)//右前电机后退
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_YQ2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ1);
	
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YQ2);
	}
}

//左后电机
void MOTO_ZH(char state)
{
	if(state == GO)//左后电机前进
	{
		GPIO_SetBits(GPIOD,MOTO_ZH2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZH1);

	}
	if(state == BACK)//左后电机后退
	{

		GPIO_SetBits(MOTO_Q_PORT,MOTO_ZH1);
	  GPIO_ResetBits(GPIOD,MOTO_ZH2);
	
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(MOTO_Q_PORT,MOTO_ZH1);
	  GPIO_ResetBits(GPIOD,MOTO_ZH2);
	}
		
}

//右后电机
void MOTO_YH(char state)
{
	if(state == GO)//右后电机前进
	{
		GPIO_SetBits(MOTO_Q_PORT,MOTO_YH2);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH1);
		
	}
	if(state == BACK)//右后电机后退
	{

		GPIO_SetBits(MOTO_Q_PORT,MOTO_YH1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH2);
	}
	if(state == STOP)//停转
	{
		GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH1);
	  GPIO_ResetBits(MOTO_Q_PORT,MOTO_YH2);
	}
		
}


//***************************前进***************************//
//只要配置INx()的状态就可以改变电机转动方向
void Car_Go(void)
{
	//左前电机 前    //右前电机 前
	MOTO_ZQ(GO);       MOTO_YQ(GO);

	//左后电机 前   //右后电机 前
	MOTO_ZH(GO);       MOTO_YH(GO);
	
	
}

void Car_Back(void)
{
		//左前电机 后    //右前电机 后
MOTO_ZQ(BACK);       MOTO_YQ(BACK);

	//左后电机 后   //右后电机 后
MOTO_ZH(BACK);       MOTO_YH(BACK);
}

//***************************向左转圈***************************//
void Car_Turn_Left(void)
{
	
	//左前电机 后    //右前电机 前
MOTO_ZQ(BACK);       MOTO_YQ(GO);

	//左后电机 后   //右后电机 前
MOTO_ZH(BACK);       MOTO_YH(GO);
	
}


//***************************向右转圈***************************//
void Car_Turn_Right(void)
{
	//左前电机 前    //右前电机 后
MOTO_ZQ(GO);       MOTO_YQ(BACK);

	//左后电机 前   //右后电机 后
  MOTO_ZH(GO);       MOTO_YH(BACK);
	
}
//**************************向右平移************************//
void Car_GO_Right(void)
{
//左前电机 后      //右前电机 前
MOTO_ZQ(BACK);    MOTO_YQ(GO);
//左后电机  前     //右后电机 后
MOTO_ZH(GO);      MOTO_YH(BACK);

}

//**************************向左平移************************//
void Car_GO_Left(void)
{
//左前电机 前      //右前电机 后
MOTO_ZQ(GO);    MOTO_YQ(BACK);
//左后电机  后     //右后电机 前
MOTO_ZH(BACK);      MOTO_YH(GO);

}



//***************************停车***************************//
void Car_Stop(void)
{
	//左前电机 停    //右前电机 停
MOTO_ZQ(STOP);       MOTO_YQ(STOP);

	//左后电机 停   //右后电机 停
  MOTO_ZH(STOP);       MOTO_YH(STOP);
}



