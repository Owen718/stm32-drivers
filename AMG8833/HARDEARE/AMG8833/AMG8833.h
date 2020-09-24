#ifndef __AMG8833_H
#define __AMG8833_H
#include "sys.h"
#include "IIC.h"


u8 Init_AMG8833(void);
void read_onepixel(void);
void read_64pixel(void);
float change(u8 x);
void get_64pixel(void);
void printf_array(void);

 #endif

