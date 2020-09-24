#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 			
	
int main
{
    float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�			
			
	MPU_Init();		
	while(mpu_dmp_init()){}
	
	while(1)
	{
	   mpu_dmp_get_data(&pitch,&roll,&yaw)��//��ȡ��̬�ǣ�return 0��ʾ�ɹ�
	   MPU_Get_Temperature();	//�õ��¶�ֵ
	   MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
	   MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	}
}			
	