#ifndef _Barometer_H_
#define _Barometer_H_


#include "stm32f10x.h"
#include "Sensor.h"

class Barometer : public Sensor{
protected:
	float _Baro_Origin;
public:
	virtual bool Initialize()=0;
	virtual bool Update(float &pressure)=0;
	float BaroOrigin(){return _Baro_Origin;}
	
};








#endif
