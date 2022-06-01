#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "stm32f10x.h"
#include "SysTime.h"

class Sensor{
protected:
	bool _mHealthy;
	bool _mIsUpdated;
	double _mUpdatedTime;
	double _mInterval;
public:
	Sensor(){
		_mHealthy=false;
		_mIsUpdated=true;
		_mUpdatedTime=0;
		_mInterval=0;
	}
	void Updated(){
		_mIsUpdated=true;
		double currentTime = SysTime::Now();
		_mInterval = currentTime - _mUpdatedTime;
		_mUpdatedTime = currentTime;
	}
	float Interval(){
		return _mInterval;
	}
	bool IsHealthy(){
		return _mHealthy;
	}


};











#endif
