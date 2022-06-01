#include "DCBLMotor.h"

DCBLMotor::DCBLMotor(Timer &_Motor_,u8 Number):_Motor(_Motor_),number(Number){
}

void DCBLMotor::SetThrottle(float Percent){
	if(0)//电机使能
	_Motor.SetDutyRadio(number,Percent);
}

