#ifndef _SAFETY_H_
#define _SAFETY_H_

#include "stm32f10x.h"
#include "UART.h"
#include "UIDLink.h"
#include "SBUS.h"
#include "MPU6050.h"
extern uint8_t _fly_state;
extern float pitch,roll,yaw;
extern SBUS _sbus;
extern float Vol;
extern Vector3f Acc,Gyro,Mag;
class Safety{
	public:
	static int safe;	
	static void Initialize();
	static void updata();
};

#endif 
