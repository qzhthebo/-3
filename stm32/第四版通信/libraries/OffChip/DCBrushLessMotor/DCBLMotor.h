#ifndef _DCBLMOTOR_H_
#define _DCBLMOTOR_H_

#include "stm32f10x.h"
#include "Timer.h"
#include "SBUS.h"


class DCBLMotor{
private:
		Timer &_Motor;
		u8 number;
public:
		DCBLMotor(Timer &_Motor_,u8 Number);
		void SetThrottle(float Percent);
};


#endif
