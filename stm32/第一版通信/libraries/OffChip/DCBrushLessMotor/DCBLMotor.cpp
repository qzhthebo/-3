#include "DCBLMotor.h"

DCBLMotor::DCBLMotor(Timer &_Motor_,u8 Number):_Motor(_Motor_),number(Number){
}

void DCBLMotor::SetThrottle(float Percent){
	if(0)//���ʹ��
	_Motor.SetDutyRadio(number,Percent);
}

