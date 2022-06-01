#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_


#define COM_PID_PARAMS				0x01
#define COM_FLY_DATA					0x02


//============================		data related file
#include "stm32f10x.h"
#include "UIDLink.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "MS5611.h"
#include "SBUS.h"
#include "RemoteControl.h"
#include "Control.h"
#include "IMU_YIS100.h"

//============================     Global params
extern u8 Protocol[][3];
extern UIDLink _UIDLink;
extern SBUS _sbus;
extern RemoteControl _remote;
extern uint8_t _fly_state;
extern Vector3f Acc,Gyro,Mag;
extern float press;
extern float pitch,roll,yaw;
extern float altitude;


#pragma pack(1)


struct PID
{
	int16_t pid_param[18];
};


struct FLY_DATA
{
	uint16_t Seq;
	int16_t pitch;
	int16_t roll;	
	int16_t yaw;
	int16_t Acc_x;  
	int16_t Acc_y;
	int16_t Acc_z;
	int16_t Gyro_x;
	int16_t Gyro_y;
	int16_t Gyro_z;
	int16_t Mag_x;
	int16_t Mag_y;
	int16_t Mag_z;
	int16_t Press;
	int16_t optical_flow_x;
	int16_t optical_flow_y;
	int16_t laser_distance;
	uint16_t Rc_Channel[9];
	uint8_t Fly_state;
};
#pragma pack()







class Communication
{
public:
	static void SendData(int FnCode);
	static int CheckData();
	static int Updata();
	
};


#endif







