#ifndef _INERTIAL_SENSOR_H_
#define _INERTIAL_SENSOR_H_

#include "stm32f10x.h"
#include "Mathtools.h"
#include "Sensor.h"


#define OFFSET_AV_NUM 128

class InertialSensor : public Sensor{
protected:
	//Sensor Data Zero Offset
	Vector3<s16> _Acc_Zero_Offset;
	Vector3<s16> _Gyro_Zero_Offset;
	//Sensor Origin Data
	Vector3<int16_t> _Acc_Origin;  
	Vector3<int16_t> _Gyro_Origin;
	//Sensor Data Scale Transmitted
	Vector3f _Acc_G;
	Vector3f _Gyro;

	//Calibrate param
	bool _Gyro_Calibrating;
	Vector3<s32> _Gyro_Calibrate_Sum;
	u16 _Gyro_Calibrate_count;
	
public:
	//Sensor Initialize
	virtual bool Initialize_IMU(void) =0;
	//Sensor update data
	virtual bool Update_data(Vector3f &acc, Vector3f &gyro)=0;

	//Data Getter Origin
	s16 AccOriginX(){return _Acc_Origin.x;}
	s16 AccOriginY(){return _Acc_Origin.y;}
	s16 AccOriginZ(){return _Acc_Origin.z;}
	s16 GyroOriginX(){return _Gyro_Origin.x;}
	s16 GyroOriginY(){return _Gyro_Origin.y;}
	s16 GyroOriginZ(){return _Gyro_Origin.z;}
	Vector3<s16> AccOrigin(){return _Acc_Origin;}
	Vector3<s16> GyroOrigin(){return _Gyro_Origin;}
	
	//Data Getter Scale Transmitted
	float AccX(){return _Acc_G.x;}
	float AccY(){return _Acc_G.y;}
	float AccZ(){return _Acc_G.x;}
	float GyroX(){return _Gyro.x;}
	float GyroY(){return _Gyro.y;}
	float GyroZ(){return _Gyro.z;}
	Vector3f Acc(){return _Acc_G;}
	Vector3f Gyro(){return _Gyro;}
	
	//Data Offset
	Vector3<s16> AccOffset() {return _Acc_Zero_Offset;}
	Vector3<s16> GyroOffset() {return _Gyro_Zero_Offset;}
	
	//Offset Calling
	void StartGyroCalibrating(){
		_Gyro_Calibrating = true;
		_Gyro_Calibrate_count = 0;
		_Gyro_Calibrate_Sum.Zero();
	}
	void StopCalibrating() {_Gyro_Calibrating=false;}
	bool IstoCalibrate() {return _Gyro_Calibrating;}
	
	
};












#endif
