#ifndef __MAIN_H
#define __MAIN_H

/*---------------------------------------变量声明---------------------------------------------
宏定义声明
-----------------------------------------变量声明--------------------------------------------*/ 
#define SendTLen1 15   //6                                   //其它发送数据及接收指令的字节长度
#define SendDLen1 119  //49                                  //发送车辆数据的字节长度
#define GfeeBufC 12                                   //车辆信息缓冲指针,共10个，GfeeBufH,GfeeBufT; 

/*---------------------------------------变量声明---------------------------------------------
预定义
-----------------------------------------变量声明--------------------------------------------*/
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "wdg.h"
#include "status.h"
#include "timer.h"
#include "anjian.h"
#include "anjian_gongneng.h"
#include "stmflash.h"
#include "cs5532_code.h"
#include "LCD_display.h"
#include "chuzhou_code.h"
#include "GMXQ_IO_Init.h"
#include "jingtaicaiji.h"
#include "js_chuli.h"
#include "math.h"
#include "W_jsfx.h"

 
//CRC-16/XMODEM
#define xcrc(crc,cp) (crctab[((crc>>8) & 0xff)^(cp & 0xff)]^(crc<<8))   /////校验计算公式

//#define WEIGHT_FENGDU 50 //weight_fengduzhi

/*---------------------------------------变量声明---------------------------------------------
保存数据读出来的临时单元声明
-----------------------------------------变量声明--------------------------------------------*/
extern u8 K1_1,K1_2,K1_3,K1_4,K1_5,K1_6;//K1 系数(接线盒部分秤台与进车小秤台)
extern u8 KG_1,KG_2,KG_3,KG_4,KG_5,KG_6;//KG 标定重量   
extern u8 CZ_1,CZ_2,CZ_3,CZ_4,CZ_5,CZ_6;//CZ (重物+秤台)内码 
extern u8 KM_1,KM_2,KM_3,KM_4,KM_5,KM_6;//KM 进车小秤台轴重k系数
extern u8 CL_1,CL_2,CL_3,CL_4,CL_5,CL_6;//CL 接线盒传感器路数，检磅设置，轴数设置 
extern u8 CD_1,CD_2,CD_3,CD_4,CD_5,CD_6;//CD 秤台宽度	  0.2250
extern u8 FD_1,FD_2,FD_3,FD_4,FD_5,FD_6;//FD 重量分度值      50
extern u8 YC_1,YC_2,YC_3,YC_4,YC_5,YC_6;//YC 一车一杆次数与范围设置
extern u8 BH_1,BH_2,BH_3,BH_4,BH_5,BH_6;//BH 仪表编号 
extern u8 YL_1,YL_2,YL_3,YL_4,YL_5,YL_6;//YL 预留

extern u8 X1_1,X1_2,X1_3,X1_4,X1_5,X1_6;//X1 进车小秤台零点内码
extern u8 X2_1,X2_2,X2_3,X2_4,X2_5,X2_6;//X2 接线盒秤台零点内码
extern u8 Z1_1,Z1_2,Z1_3,Z1_4,Z1_5,Z1_6;//Z1 进车小轮轴一零点内码
extern u8 Z2_1,Z2_2,Z2_3,Z2_4,Z2_5,Z2_6;//Z2 进车小轮轴二零点内码
extern u8 Z3_1,Z3_2,Z3_3,Z3_4,Z3_5,Z3_6;//Z3 下磅小轮轴一零点内码
extern u8 Y1_1,Y1_2,Y1_3,Y1_4,Y1_5,Y1_6;//Y1 进车小轮轴一阈值
extern u8 Y2_1,Y2_2,Y2_3,Y2_4,Y2_5,Y2_6;//Y2 进车小轮轴二阈值
extern u8 Y3_1,Y3_2,Y3_3,Y3_4,Y3_5,Y3_6;//Y3 下磅小轮轴一阈值
extern u8 CS_1,CS_2,CS_3,CS_4,CS_5,CS_6;//CS 出车修改倍数设置
extern u8 U1_1,U1_2,U1_3,U1_4,U1_5,U1_6;//U1 出车2轴车整车重量修正      +0.0000

extern u8 U2_1,U2_2,U2_3,U2_4,U2_5,U2_6;//U2 出车3、4轴车整车重量修正   +0.0000
extern u8 U3_1,U3_2,U3_3,U3_4,U3_5,U3_6;//U3 出车5、6轴车整车重量修正   +0.0000
extern u8 V1_1,V1_2,V1_3,V1_4,V1_5,V1_6;//V1 整车5~=7公里出车整车速度修正系数	+0.0000
extern u8 V2_1,V2_2,V2_3,V2_4,V2_5,V2_6;//V2 整车7~=9公里出车整车速度修正系数	+0.0000
extern u8 V3_1,V3_2,V3_3,V3_4,V3_5,V3_6;//V3 整车9~=11公里出车整车速度修正系数	+0.0000
extern u8 V4_1,V4_2,V4_3,V4_4,V4_5,V4_6;//V4 整车11~=13公里出车整车速度修正系数	+0.0000
extern u8 V5_1,V5_2,V5_3,V5_4,V5_5,V5_6;//V5 整车13~=15公里出车整车速度修正系数	+0.0000
extern u8 V6_1,V6_2,V6_3,V6_4,V6_5,V6_6;//V6 整车15~=17公里出车整车速度修正系数	+0.0000
extern u8 V7_1,V7_2,V7_3,V7_4,V7_5,V7_6;//V7 整车17~=19公里出车整车速度修正系数	+0.0000
extern u8 V8_1,V8_2,V8_3,V8_4,V8_5,V8_6;//V8 整车>19公里出车整车速度修正系数	+0.0000

extern u8 H1_1,H1_2,H1_3,H1_4,H1_5,H1_6;//H1 进车小秤台34轴车速度重量修正，<=5公里        +0.0000
extern u8 H2_1,H2_2,H2_3,H2_4,H2_5,H2_6;//H2 进车小秤台34轴车速度重量修正，5~=8公里       +0.0000
extern u8 H3_1,H3_2,H3_3,H3_4,H3_5,H3_6;//H3 进车小秤台34轴车速度重量修正，8~=10          +0.0000
extern u8 H4_1,H4_2,H4_3,H4_4,H4_5,H4_6;//H4 进车小秤台34轴车速度重量修正，10~=12         +0.0000
extern u8 H5_1,H5_2,H5_3,H5_4,H5_5,H5_6;//H5 进车小秤台34轴车速度重量修正，12~=15         +0.0000
extern u8 H6_1,H6_2,H6_3,H6_4,H6_5,H6_6;//H6 进车小秤台34轴车速度重量修正，15~=17公里     +0.0000	
extern u8 H7_1,H7_2,H7_3,H7_4,H7_5,H7_6;//H7 进车小秤台34轴车速度重量修正，17~=20         +0.0000
extern u8 H8_1,H8_2,H8_3,H8_4,H8_5,H8_6;//H9 进车小秤台34轴车速度重量修正，>20公里        +0.0000	
extern u8 F1_1,F1_2,F1_3,F1_4,F1_5,F1_6;//F1 进车小秤台56轴车速度重量修正，<=5公里        +0.0000
extern u8 F2_1,F2_2,F2_3,F2_4,F2_5,F2_6;//F2 进车小秤台56轴车速度重量修正，5~=8公里       +0.0000

extern u8 F3_1,F3_2,F3_3,F3_4,F3_5,F3_6;//F3 进车小秤台56轴车速度重量修正，8~=10          +0.0000
extern u8 F4_1,F4_2,F4_3,F4_4,F4_5,F4_6;//F4 进车小秤台56轴车速度重量修正，10~=12         +0.0000
extern u8 F5_1,F5_2,F5_3,F5_4,F5_5,F5_6;//F5 进车小秤台56轴车速度重量修正，12~=15         +0.0000
extern u8 F6_1,F6_2,F6_3,F6_4,F6_5,F6_6;//F6 进车小秤台56轴车速度重量修正，15~=17公里     +0.0000	
extern u8 F7_1,F7_2,F7_3,F7_4,F7_5,F7_6;//F7 进车小秤台56轴车速度重量修正，17~=20         +0.0000
extern u8 F8_1,F8_2,F8_3,F8_4,F8_5,F8_6;//F8 进车小秤台56轴车速度重量修正，>20公里        +0.0000
extern u8 L1_1,L1_2,L1_3,L1_4,L1_5,L1_6;//L1 进车小秤台2轴车速度重量修正，5~=8公里        +0.0000
extern u8 L2_1,L2_2,L2_3,L2_4,L2_5,L2_6;//L2 进车小秤台2轴车速度重量修正，8~=10           +0.0000
extern u8 L3_1,L3_2,L3_3,L3_4,L3_5,L3_6;//L3 进车小秤台2轴车速度重量修正，10~=12          +0.0000
extern u8 L4_1,L4_2,L4_3,L4_4,L4_5,L4_6;//L4 进车小秤台2轴车速度重量修正，12~=15         +0.0000

extern u8 L5_1,L5_2,L5_3,L5_4,L5_5,L5_6;//L5 进车小秤台2轴车速度重量修正，15~=17         +0.0000
extern u8 L6_1,L6_2,L6_3,L6_4,L6_5,L6_6;//L6 进车小秤台2轴车速度重量修正，17~=20公里     +0.0000	
extern u8 L7_1,L7_2,L7_3,L7_4,L7_5,L7_6;//L7 进车小秤台2轴车速度重量修正，>20公里         +0.0000
extern u8 J2_1,J2_2,J2_3,J2_4,J2_5,J2_6;//J2 2轴车轴间距设置         02.6000
extern u8 J3_1,J3_2,J3_3,J3_4,J3_5,J3_6;//J3 3轴车轴间距设置         02.5000
extern u8 J4_1,J4_2,J4_3,J4_4,J4_5,J4_6;//J4 4轴车轴间距设置         02.8000
extern u8 J5_1,J5_2,J5_3,J5_4,J5_5,J5_6;//J5 5轴车轴间距设置         02.8000
extern u8 J6_1,J6_2,J6_3,J6_4,J6_5,J6_6;//J6 6轴车轴间距设置         01.5000
extern u8 A1_1,A1_2,A1_3,A1_4,A1_5,A1_6;//A1 进车小秤台12路零点（未加差值）
extern u8 A2_1,A2_2,A2_3,A2_4,A2_5,A2_6;//A2 进车小秤台34路零点（未加差值）

extern u8 B7_1,B7_2,B7_3,B7_4,B7_5,B7_6;//B7   进车小秤台第一路标平差值，不在参数里显示
extern u8 B8_1,B8_2,B8_3,B8_4,B8_5,B8_6;//B8   进车小秤台第二路标平差值，不在参数里显示
extern u8 B9_1,B9_2,B9_3,B9_4,B9_5,B9_6;//B9   进车小秤台第一、二路的最大值，不在参数里显示


extern float k_xishu;                   //K1   K系数                1.0000  //k系数=标定重量值/(AD_data-weight_x0)
extern u32 weight_biaoding;             //KG   保存标定重量值       1000
extern u32 weight_biaoding_ad;          //CZ   标定重量标定AD值     0
extern float M_xishu;                   //KM  进车小秤台重量与接线盒秤台重量比例

//extern u8 lz_sz;                         //CL_2    =0使用小轮轴作为出车轴数，=1使用小秤台作为出车轴数
extern u8 jxh_lushu;                     //CL_3*10+CL_4   设置接线盒接入几路传感器路数
extern u8 gn_sz;                         //CL_5   控制位  0：正常使用  1：滑动次数加多检磅  2：标检           50
extern u8 ZHOUSHU;                       //CL_6   保存轴数        0

extern float CHTKD;                     //CD 秤台宽度
extern u32 WEIGHT_FENGDU;               //FD  分度值设置

extern u32 yc_cs;                       //YC
extern u32 yc_zl;                       //YC

extern u32 yibiao_bh;                   //BH   仪表编号
//extern u32 yibiao_YL;                   //YL
extern float yibiao_YL;                   //YL
extern u32 weight_x0_1;                 //X1   进车小秤台零点
extern u32 weight_x0;                   //X2   接线盒秤台零点         
extern u32 JC_XIAO_AD;                  //Z1   进车辅轮轴AD初始值
extern u32 jc_xlz2;                     //Z2   进车小轮轴2零点值
extern u32 CC_XIAO_AD;                  //Z3   下磅辅轮轴AD初始值
extern u32 JINCHE_ZFLZ_threshold;       //Y1   进车辅轮轴1AD阈值   
extern u32 jc_xlz2_threshold;           //Y2   进车小轮轴2阈值
extern u32 CHUCHE_ZFLZ_threshold;       //Y3   下磅辅轮轴1阈值   

extern u8 JC12_flag;	                   //CS   进车小秤台第一路标平标志
extern u8 chuche_xy300;                 //CS   出车小于200公斤，进车小秤台修正系数
extern u8 JC34_flag;	                   //CS   进车小秤台第二路标平标志
extern u8 chuche_dy300;                 //CS   出车大于200公斤，进车小秤台修正系数

extern float U1_xzh;                    //U1   2轴整车重量修正         0.0000
extern float U2_xzh;                    //U2   3、4轴整车重量修正      0.0000
extern float U3_xzh;                    //U3   5、6轴整车重量修正      0.0000

extern float v1_xzh;                      //v1 	整车速度对重量修正，5~=7公里        0.0000
extern float v2_xzh;                      //v2   整车速度对重量修正，7~=9公里        0.0000
extern float v3_xzh;                      //v3 	整车速度对重量修正，9~=11公里       0.0000
extern float v4_xzh;                      //v4 	整车速度对重量修正，11~=13公里      0.0000
extern float v5_xzh;                      //v5   整车速度对重量修正，13~=15公里      0.0000
extern float v6_xzh;                      //v6	整车速度对重量修正，15~=17公里      0.0000
extern float v7_xzh;                      //v7   整车速度对重量修正，17~=19公里      0.0000
extern float v8_xzh;                      //v8	整车速度对重量修正，>5公里          0.0000

extern float H1_xzh;                      //H1   进车小秤台34轴速度对重量修正，<=5公里        0.0000
extern float H2_xzh;                      //H2   进车小秤台34轴速度对重量修正，5~=8公里       0.0000
extern float H3_xzh;                      //H3   进车小秤台34轴速度对重量修正，8~=10公里      0.0000
extern float H4_xzh;                      //H4   进车小秤台34轴速度对重量修正，10~=12公里     0.0000
extern float H5_xzh;                      //H5   进车小秤台56轴速度对重量修正，12~=15公里     0.0000
extern float H6_xzh;                      //H6   进车小秤台56轴速度对重量修正，15~=17公里     0.0000
extern float H7_xzh;                      //H7   进车小秤台56轴速度对重量修正，17~=20公里     0.0000
extern float H8_xzh;                      //H8   进车小秤台56轴速度对重量修正，>20公里        0.0000

extern float F1_xzh;                      //F1   进车小秤台34轴速度对重量修正，<=5公里        0.0000  
extern float F2_xzh;                      //F2   进车小秤台34轴速度对重量修正，5~=8公里       0.0000
extern float F3_xzh;                      //F3   进车小秤台34轴速度对重量修正，8~=10公里      0.0000
extern float F4_xzh;                      //F4   进车小秤台34轴速度对重量修正，10~=12公里     0.0000
extern float F5_xzh;                      //F5   进车小秤台56轴速度对重量修正，12~=15公里     0.0000
extern float F6_xzh;                      //F6   进车小秤台56轴速度对重量修正，15~=17公里     0.0000
extern float F7_xzh;                      //F7   进车小秤台56轴速度对重量修正，17~=20公里     0.0000
extern float F8_xzh;                      //F8   进车小秤台56轴速度对重量修正，>20公里        0.0000

extern float L1_xzh;                      //L1   进车小秤台2轴速度对重量修正，5~=8公里        0.0000
extern float L2_xzh;                      //L2   进车小秤台2轴速度对重量修正，8~=10公里       0.0000
extern float L3_xzh;                      //L3   进车小秤台2轴速度对重量修正，10~=12公里      0.0000
extern float L4_xzh;                      //L4   进车小秤台2轴速度对重量修正，12~=15公里      0.0000
extern float L5_xzh;                      //L5   进车小秤台2轴速度对重量修正，15~=17公里      0.0000
extern float L6_xzh;                      //L6   进车小秤台2轴速度对重量修正，17~=20公里      0.0000
extern float L7_xzh;                      //L7   进车小秤台2轴速度对重量修正，>20公里         0.0000

extern float JIANJU_2_PANDUAN;            //J2   2轴车轴间距设置
extern float JIANJU_3_PANDUAN;            //J3   3轴车轴间距设置
extern float JIANJU_4_PANDUAN;            //J4   4轴车轴间距设置
extern float JIANJU_5_PANDUAN;            //J5   5轴车轴间距设置
extern float JIANJU_6_PANDUAN;            //J6   6轴车轴间距设置

extern u32 JC12_AD;						  //A1   进车小秤台12路零点
extern u32 JC34_AD;	                      //A2	进车小秤台34路零点

extern u8 state_cl;//状态处理  动静
extern u8 chl_jsq;//下位机缓存车辆计数器

extern u16 U3_jsq;//串口3状态发送延时计数器

#endif 





































