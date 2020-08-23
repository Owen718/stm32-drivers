#include "sys.h"

extern int8_t OpenMV_RX;
extern int8_t OpenMV_RY;
extern int8_t OpenMV_GX;
extern int8_t OpenMV_GY;
extern int8_t OpenMV_add;
extern int openmv[8];//stm32接收数据数组




void Openmv_Receive_Data(int16_t data);
void Openmv_Data(void);

