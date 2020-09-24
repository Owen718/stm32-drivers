#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 			
	
int main
{
    float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度			
			
	MPU_Init();		
	while(mpu_dmp_init()){}
	
	while(1)
	{
	   mpu_dmp_get_data(&pitch,&roll,&yaw)；//获取姿态角，return 0表示成功
	   MPU_Get_Temperature();	//得到温度值
	   MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
	   MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
	}
}			
	