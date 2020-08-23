#include "openmv.h"
int openmv[8];            //stm32接收数据数组
int8_t OpenMV_RX;          /*OPENMV X 轴反馈坐标*/
int8_t OpenMV_RY;          /*OPENMV X 轴反馈坐标*/
int8_t OpenMV_GX;
int8_t OpenMV_GY;
int8_t OpenMV_add;

int i=0;

void Openmv_Receive_Data(int16_t data)    //接收Openmv传过来的数据
{
	static u8 state = 0;
	if(state==0&&data==0xb3)
	{
		state=1;
		openmv[0]=data;
	}
	else if(state==1&&data==0xb3)
	{
		state=2;
		openmv[1]=data;
	}
	else if(state==2)
	{
		state=3;
		openmv[2]=data;
	}
	else if(state==3)
	{
		state = 4;
		openmv[3]=data;
	}
	else if(state==4)
	{
		state = 5;
		openmv[4]=data;
	}
		else if(state==5)
	{
		state = 6;
		openmv[5]=data;
	}
		else if(state==6)
	{
		state = 7;
		openmv[6]=data;
	}
	else if(state==7)		//检测是否接受到结束标志
	{
        if(data == 0x5B)
        {
            state = 0;
            openmv[7]=data;
            Openmv_Data();
			data=0;
        }
        else if(data != 0x5B)
        {
            state = 0;
            for(i=0;i<8;i++)
            {
                openmv[i]=0x00;
            }           
        }
	}    
	else
		{
			state = 0;
            data=0;
			for(i=0;i<8;i++)
            {
                openmv[i]=0x00;
            }
		}
}

void Openmv_Data(void)
{
    OpenMV_RX=openmv[2];
    OpenMV_RY=openmv[3];
	  OpenMV_GX=openmv[4];
    OpenMV_GY=openmv[5];
	  OpenMV_add=openmv[6];

} 

