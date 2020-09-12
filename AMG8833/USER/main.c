#include "sys.h" 
#include "usart.h"     
#include "delay.h"
#include "IO_init.h"
#include "timer.h"
#include "stmflash.h"
 #include "IIC.h"
#include "AMG8833.h" 
#include "HMIsend.h"

void IO_init(void);	
u32  data_DL=0,Res_DL=0;
u16 send_i=0;
extern u32 TIME2_1;
extern u16 USART2_jsq;
u16 DL[100]={0};
u8 DL_i=0,DL_bj=0,DL_bj1=0,DL_j=0;
u8 flag_key1=0,flag_key2=0;	//����1/2������־

extern u8 flag_wendu;   //
u32 xh_jsq=0;
u16 for_i=0,for_j=0;
u16 hang=0,lie=0,huatu_h=0,huatu_l=0;
extern float print_buf[64];				//�����¶�ֵ��Ԫ����
extern float print_buf_max[64];		//�Ƚ��¶�ֵ��С��Ԫ����
float print_buf_int[576];					//�Ŵ�24*24��������
float print_buf_int1[576];				//�Ŵ�24*24��������1
u32 QS=0;													//��ɫֵ
char str_l[3];										//��ͼ���ַ���
char str_h[3];										//��ͼ���ַ���
char str_qs[5];										//��ͼ��ɫ�ַ���
float X1=13;											//13-20��ʹ��1984-2014Ϳɫ
float X2=20;
float Y1=1984;
float Y2=2014;

float X3=20;										//20-25��ʹ65472-65502Ϳɫ
float X4=25;
float Y3=65472;
float Y4=65502;

float X5=25;									//25-30��ʹ48222-48192Ϳɫ
float X6=30;
float Y5=48222;
float Y6=48192;

float X7=30;								//30-40��ʹ63518-63488Ϳɫ
float X8=40;
float Y7=63518;
float Y8=63488;

float K1_xishu=0;							//13-20�� ����Kϵ��
float B1_xishu=0;							//13-20�� ����Bϵ��
float K2_xishu=0;							
float B2_xishu=0;
float K3_xishu=0;
float B3_xishu=0;
float K4_xishu=0;
float B4_xishu=0;
float temp_max=0;
float temp_wendu=0;

int main(void) 
{ 
	Stm32_Clock_Init(9);  //72M 
	delay_init(72);        //��ʱ��ʼ�� 
	uart_init(72,115200);     //���ô���1������ 
	USART2_Init(36,115200);			 /*����2��ʼ��*/
	USART3_Init(36,4800);			 /*����3��ʼ��*/
	TIM2_Int_Init(99,7199);		     /*��ʱ��2��ʼ�� ��ʱ10ms*/
	TIM3_Int_Init(99,7199);			 /*��ʱ��3��ʼ�� ��ʱ10ms*/
	//JTAG_Set(0X02);	   		//GPIO��ʼ��֮PB3/PB4/PA13/PA14/PA15���ŵĸ���
	IO_init();				//IO�ڳ�ʼ��
	//DS1302IO_Init();//DS1302�˿ڳ�ʼ��
	IIC_Init();						//IIC��ʼ��
	Init_AMG8833();				//AMG8833��ʼ��
	K1_xishu=(Y2-Y1)/(X2-X1);			//����13-20�� ����Kϵ��
	B1_xishu=Y1-K1_xishu*X1;			//����13-20�� ����Bϵ��
	K2_xishu=(Y4-Y3)/(X4-X3);
	B2_xishu=Y3-K2_xishu*X3;
	K3_xishu=(Y6-Y5)/(X6-X5);
	B3_xishu=Y5-K3_xishu*X5;
	K4_xishu=(Y8-Y7)/(X8-X7);
	B4_xishu=Y7-K4_xishu*X7;
	//printf("K1=%.2f K2=%.2f K3=%.2f",K1_xishu,K2_xishu,K3_xishu);			//��ӡ�����ϵ��
	//printf("\r\n");
	//printf("B1=%.2f B2=%.2f B3=%.2f ",B1_xishu,B2_xishu,B3_xishu);
	//printf("\r\n");
	LED1=0;								//��˸led1һ�� ������ʼ�����
	delay_ms(1000);
	LED1=1;
	HMISendstart();          //Ϊȷ������HMI����ͨ��
	HMISends("cls BLACK");		//��ӱ�������ɫ
	HMISendb(0xff);
	//delay_ms(10);
	HMISends("page 4");			//ת��ҳ��
	HMISendb(0xff);
	
	while(1) 
	{
		//20180817+����ѭ��1000��
		xh_jsq++;					//ѭ������
		if(xh_jsq>=10000)
		{
		    xh_jsq=0;
				LED1=~LED1;
			flag_wendu=1;		//����ת��1000�� �ɼ�һ��
				//read_64pixel();
				//printf_array();
				//printf("\r\n");
		}
		if(flag_wendu==1)			//��־Ϊ1�����¶Ȳɼ�
		{
			//double tem ;  //���϶�
			//tem = GetTemperature1();
			//printf("present temp:%lf\r\n",tem);
			read_64pixel();				//�ɼ��¶�ֵ
			//printf_array();			//��ӡ
			//printf("\r\n");
			//size_max=0;
			//print_buf_max[size_max]=print_buf[0];
			
			for(for_i=0;for_i<64;for_i++)									//ð�ݷ��Ƚϴ�С
			{
				for(for_j=0;for_j<63-for_i;for_j++)
				{
					if(print_buf_max[for_j]<print_buf_max[for_j+1])
					{
						temp_max=print_buf_max[for_j+1];
						print_buf_max[for_j+1]=print_buf_max[for_j];
						print_buf_max[for_j]=temp_max;
					}
				}
			}
			temp_wendu=0;
			for(for_i=0;for_i<10;for_i++)						//ǰ10����ֵ�����ۼ�
			{
				temp_wendu=temp_wendu+print_buf_max[for_i];			//�����ۼ�
			}
			temp_wendu=temp_wendu/10;							//ƽ��
			temp_wendu=temp_wendu*1.5;						//��������ϵ��
			HMISends("t1.txt=\"");								//����Һ������ʾ���ղ���¶�
			sprintf(str_l,"%.2f",temp_wendu);
			HMISends(str_l);
			HMISends("\"");
			HMISendb(0xff);
			//����Ϊ��ͼ����
			for(for_i=0;for_i<576;for_i++)				//���зŴ���ʾ24*24����
			{
				hang=for_i/24;					//��������
			  lie=for_i%24;						//��������
				//if(lie==0)
				//{printf("\r\n");}
				if(hang%3==0)					//���������3�ı���
				{
					if(lie%3==0)				
					{
						print_buf_int[for_i]=print_buf[(hang/3)*8+lie/3];
					}
					if(lie%3==1)
					{
						print_buf_int[for_i]=print_buf[(hang/3)*8+lie/3]-(print_buf[(hang/3)*8+lie/3]-print_buf[(hang/3)*8+lie/3+1])/3;
					}
					if(lie%3==2)
					{
						print_buf_int[for_i]=print_buf[(hang/3)*8+lie/3]-(print_buf[(hang/3)*8+lie/3]-print_buf[(hang/3)*8+lie/3+1])*2/3;
					}
				}
				if(hang%3==1)
				{
					if(lie%3==0)
					{
						print_buf_int[for_i]=print_buf[(hang/3)*8+lie/3]-(print_buf[(hang/3)*8+lie/3]-print_buf[(hang/3)*8+lie/3+8])/3;
					}
				}
				if(hang%3==2)
				{
					if(lie%3==0)
					{
						print_buf_int[for_i]=print_buf[(hang/3)*8+lie/3]-(print_buf[(hang/3)*8+lie/3]-print_buf[(hang/3)*8+lie/3+8])*2/3;
					}
				}
			//printf("%.2f ",print_buf_int[for_i]);	
			}
			for(for_i=0;for_i<576;for_i++)  //24*24����
			{
				hang=for_i/24;
			  lie=for_i%24;
				//if(lie==0)
				//{printf("\r\n");}
				if(hang%3==1)
				{
					if(lie%3==1)
					{
						print_buf_int[for_i]=((print_buf_int[for_i-1]-(print_buf_int[for_i-1]-print_buf_int[for_i+2])/3)+(print_buf_int[for_i-24]-(print_buf_int[for_i-24]-print_buf_int[for_i+48])/3))/2;
						//print_buf_int[for_i]=print_buf_int[for_i-25]-(print_buf_int[for_i-25]-print_buf_int[for_i+50])/3;
						//print_buf_int[for_i]=print_buf_int[for_i-24]-(print_buf_int[for_i-24]-(print_buf[(hang/3+1)*8+lie/3+1]-(print_buf[(hang/3+1)*8+lie/3+1]-print_buf[(hang/3+1)*8+lie/3+1+8])/3))/3;
					}
					if(lie%3==2)
					{
						print_buf_int[for_i]=((print_buf_int[for_i-2]-(print_buf_int[for_i-2]-print_buf_int[for_i+1])*2/3)+(print_buf_int[for_i-24]-(print_buf_int[for_i-24]-print_buf_int[for_i+48])/3))/2;
						//print_buf_int[for_i]=print_buf_int[for_i-24]-(print_buf_int[for_i-24]-(print_buf[(hang/3+1)*8+lie/3+2]-(print_buf[(hang/3+1)*8+lie/3+2]-print_buf[(hang/3+1)*8+lie/3+2+8])*2/3))/3;
					}
				}
				if(hang%3==2)
				{
					if(lie%3==1)
					{
						print_buf_int[for_i]=((print_buf_int[for_i-1]-(print_buf_int[for_i-1]-print_buf_int[for_i+2])/3)+(print_buf_int[for_i-48]-(print_buf_int[for_i-48]-print_buf_int[for_i+24])*2/3))/2;
						//print_buf_int[for_i]=print_buf_int[for_i-24]-(print_buf_int[for_i-24]-(print_buf[(hang/3+1)*8+lie/3+1]-(print_buf[(hang/3+1)*8+lie/3+1]-print_buf[(hang/3+1)*8+lie/3+1+8])/3))*2/3;
					}
					if(lie%3==2)
					{
						print_buf_int[for_i]=((print_buf_int[for_i-2]-(print_buf_int[for_i-2]-print_buf_int[for_i+1])*2/3)+(print_buf_int[for_i-48]-(print_buf_int[for_i-48]-print_buf_int[for_i+24])*2/3))/2;
					}
				}
				//printf("%.2f ",print_buf_int[for_i]);
				
				
				switch(hang)					//ѡ��Ҫ��ʾ������
				{
					case 0: huatu_h=0;break;
					case 1: huatu_h=8;break;
					case 2: huatu_h=16;break;
					case 3: huatu_h=24;break;
					case 4: huatu_h=32;break;
					case 5: huatu_h=40;break;
					case 6: huatu_h=48;break;
					case 7: huatu_h=56;break;
					case 8: huatu_h=64;break;
					case 9: huatu_h=72;break;
					case 10: huatu_h=80;break;
					case 11: huatu_h=88;break;
					case 12: huatu_h=96;break;
					case 13: huatu_h=104;break;
					case 14: huatu_h=112;break;
					case 15: huatu_h=120;break;
					case 16: huatu_h=128;break;
					case 17: huatu_h=136;break;
					case 18: huatu_h=144;break;
					case 19: huatu_h=152;break;
					case 20: huatu_h=160;break;
					case 21: huatu_h=168;break;
					case 22: huatu_h=176;break;
					case 23: huatu_h=184;break;
					default:break;
				}
				switch(lie)					//ѡ��Ҫ��ʾ������
				{
					case 0: huatu_l=0;break;
					case 1: huatu_l=8;break;
					case 2: huatu_l=16;break;
					case 3: huatu_l=24;break;
					case 4: huatu_l=32;break;
					case 5: huatu_l=40;break;
					case 6: huatu_l=48;break;
					case 7: huatu_l=56;break;
					case 8: huatu_l=64;break;
					case 9: huatu_l=72;break;
					case 10: huatu_l=80;break;
					case 11: huatu_l=88;break;
					case 12: huatu_l=96;break;
					case 13: huatu_l=104;break;
					case 14: huatu_l=112;break;
					case 15: huatu_l=120;break;
					case 16: huatu_l=128;break;
					case 17: huatu_l=136;break;
					case 18: huatu_l=144;break;
					case 19: huatu_l=152;break;
					case 20: huatu_l=160;break;
					case 21: huatu_l=168;break;
					case 22: huatu_l=176;break;
					case 23: huatu_l=184;break;
					default:break;
				}
				if(print_buf_int[for_i]>=10 && print_buf_int[for_i]<20)			//10-20�ȼ�����ɫֵ
				{
					QS=print_buf_int[for_i]*K1_xishu+B1_xishu;
					//printf("%d ",QS);
				}
				if(print_buf_int[for_i]>=20 && print_buf_int[for_i]<25)			//20-25�ȼ�����ɫֵ
				{
					QS=print_buf_int[for_i]*K2_xishu+B2_xishu;
					//printf("%d ",QS);
				}
				if(print_buf_int[for_i]>=25 && print_buf_int[for_i]<30)	//25-30�ȼ�����ɫֵ
				{
					QS=print_buf_int[for_i]*K3_xishu+B3_xishu;
					//printf("%d ",QS);
				}
				if(print_buf_int[for_i]>=30 && print_buf_int[for_i]<40)	//30-40�ȼ�����ɫֵ
				{
					QS=print_buf_int[for_i]*K4_xishu+B4_xishu;
					//printf("%d ",QS);
				}
				if(print_buf_int[for_i]!=print_buf_int1[for_i])		//����һ�������ظ�����ʾ
				{
					if(lie<=21 && hang<=21)		//22,23�в���ʾ  22��23�в���ʾ
					{
						print_buf_int1[for_i]=print_buf_int[for_i];	//���������ݸ�ֵ����������
						huatu_h=huatu_h+48;
						huatu_l=huatu_l+20;
						HMISends("fill ");
						sprintf(str_l,"%d",huatu_l);		//��λ����
						HMISends(str_l);
						HMISends(",");
						sprintf(str_h,"%d",huatu_h);			//��λ����
						HMISends(str_h);
						HMISends(",8,8,");				//��ʾ8*8�����С
						sprintf(str_qs,"%d",QS);		//��ʾ��ɫֵ
						HMISends(str_qs);
						HMISendb(0xff);				//������
					}
				}
			}
			flag_wendu=0;			//��־����
			LED2=~LED2;				//����˸
		}	
		
		if(KEY1==0 && flag_key1==0)
		{
			delay_ms(10);
			if(KEY1==0)
			{
				flag_key1=1;
				//USART2_Send(0x46);
				LED2=0;
				read_64pixel();
				printf_array();
				printf("\r\n");
				//POW_ON=1;CTRL=1;
				//LED2=0;
			}
		}
		else if(KEY1==1)
		{
			flag_key1=0;LED2=1;
		}
	} 
} 

