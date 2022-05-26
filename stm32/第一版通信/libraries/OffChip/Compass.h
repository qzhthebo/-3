#ifndef _COMPASS_H_
#define _COMPASS_H_

#include "stm32f10x.h"
#include "Mathtools.h"
#include "Sensor.h"

class Compass : public Sensor
{
protected:
	Vector3<int16_t> _Mag_Origin;  
public:
	virtual bool Initialize()=0;
	virtual u8 Update_data(Vector3f &magnet)=0;

	float MagOriginX(){return _Mag_Origin.x;}
	float MagOriginY(){return _Mag_Origin.y;} 
	float MagOriginZ(){return _Mag_Origin.z;} 
};


#endif

