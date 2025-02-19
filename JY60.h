#ifndef GYRO_H
#define GYRO_H
#include "mbed.h"

extern float yaw1;
extern float yaw2;
extern int GetData1;
extern int GetData2;
extern float DeltaAngle;
extern float FinishFlag;
//extern float a_x, a_y, a_z;
void StartdeviceEvent();
void EnddeviceEvent();
#endif