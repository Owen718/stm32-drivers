
typedef struct
{
	float Kp;//比例
	float Ki;//积分
	float Kd;//微分

	float Ek; //当前误差
	float Ek1;//前一次误差
	float EK2;//再前一次误差
	float LocSum; //累计积分误差
}PID_LocTypeDef;


/***************************
函数名称：PID_Loc  （位置型PID算法）
功 能：PID位置（Location）计算
参 数：SetValue ---设置值(期望值)
	  ActualValue ---实际值（反馈值）
	  PID -----PID数据结构
返回值： PIDLoc----PID位置
****************************/

float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
	float PIDLoc;  //位置
	
	PID->Ek = SetValue - ActualValue;
	PID->LocSum += PID->Ek; //累计误差          

	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek1 - PID->Ek);//当前位置=P*当前误差+I*累计积分误差+D*当前误差

	PID->Ek1 = PID->Ek;  //EK迭代为前次误差
	return PIDLoc;
}




