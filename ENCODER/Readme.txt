#include "encoder.h"
#include "timer.h"

int Encoder_left; //实际脉冲频率的四倍
int Encoder_Right；
float pulse_L;  //实际脉冲的频率
float pulse_R;
float round_L;  //转速
float round_R;

int main()
{
	Encoder_Init_TIM2();          //配置为编码器接口，捕获A、B项方波脉冲个数
                Encoder_Init_TIM4();          //四倍频
                TIM3_Int_Init(499,7199);    //定时1/20s，用于计算一定时间内脉冲数
                while（1）
                {

	}
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
  	{
                                TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 

                                if(TIM2->CR1&1<<4)     //判断计数器计数方向，向上为1，从而判断正反转
                                { printf("L_forward")；DIR_L=1；}
                                else  {printf("L_backward");  DIR_L=0；}

                                if(TIM4->CR1&1<<4)     //判断计数器计数方向，向上为1，从而判断正反转
                                { printf("R_forward")；DIR_R=1；}
                                else  {printf("R_backward");  DIR_R=0；}

		Encoder_Left=(short)20*TIM2->CNT;         //1/20s内的脉冲数的4倍
                                pulse_L = Encoder_Left/4.0f;		//由于是TIM_EncoderMode_TI12，所以要四分频，即除以四，得到一秒内实际的脉冲值
		round_L=  Encoder_Left/4.0f/260.0f;	//假设电机每转一圈，电机20转，对应13线的话，所以产生260脉冲
		TIM2->CNT=0;

                                Encoder_Right=(short)20*TIM4->CNT;         //1/20s内的脉冲数的4倍
                                pulse_R = Encoder_Right/4.0f;		//由于是TIM_EncoderMode_TI12，所以要四分频，即除以四，得到一秒内实际的脉冲值
		round_R= Encoder_Right/4.0f/260.0f;	//假设电机每转一圈，电机20转，对应13线的话，所以产生260脉冲
		TIM4->CNT=0;		
	}
}