#ifndef __HMISend_H
#define __HMISend_H
#include "sys.h"
//#include "IIC.h"
void HMISendstart(void);
void HMISends(char *buf1);		  //字符串发送函数
void HMISendb(u8 HMI_k)	;	         //字节发送函数
 #endif

