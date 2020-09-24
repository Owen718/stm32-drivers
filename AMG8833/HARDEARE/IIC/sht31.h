#ifndef __SHT31_H
#define __SHT31_H
#include "sys.h" 
#define SHT30_ADDR 0x88  //ADDR (pin 2)connected to VDD

void Read_sht31(void);
void Cmd_Write_sht31(uint8_t msb,uint8_t lsb);
int  Convert_sht31_tem(void);
int  Convert_sht31_hum(void);

#endif
