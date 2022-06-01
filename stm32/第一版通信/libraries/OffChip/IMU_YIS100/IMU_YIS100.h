#ifndef _IMU_YIS100_H_
#define _IMU_YIS100_H_

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "UART.h"
#include "Vector3.h"
#include "Serialize.h"

#pragma pack(1)
struct YISData
{
	u16 header;
	u16 TID;
	u8 LEN;
	u8 accID;
	u8 accLen;
	s32 accx;
	s32 accy;
	s32 accz;
	u8 gyroID;
	u8 gyroLen;
	s32 gyrox;
	s32 gyroy;
	s32 gyroz;
	u8 angID;
	u8 angLen;
	s32 pitch;
	s32 roll;
	s32 yaw;
	u8 sum1;
	u8 sum2;
};
#pragma pack()


class IMU_YIS100
{
public:
	UART &_com;
	FIFOBuffer<u8, 500> &_Rx_Buffer;
	bool updated;

	IMU_YIS100(UART &com);
	YISData Raw;
	Vector3f Gyro;
	Vector3f Acc;
	Vector3f Angle;
	float pitch;
	float Roll;
	float AngVel;
	float AngAcc;
	bool tagOnLine;
	int updateCnt;
public:
	int8_t Update();
};


#endif

#ifndef _IMU_YIS100_H_
#define _IMU_YIS100_H_

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "UART.h"
#include "Vector3.h"
#include "Serialize.h"

#pragma pack(1)
struct YISData
{
	u16 header;
	u16 TID;
	u8 LEN;
	u8 accID;
	u8 accLen;
	s32 accx;
	s32 accy;
	s32 accz;
	u8 gyroID;
	u8 gyroLen;
	s32 gyrox;
	s32 gyroy;
	s32 gyroz;
	u8 angID;
	u8 angLen;
	s32 pitch;
	s32 roll;
	s32 yaw;
	u8 sum1;
	u8 sum2;
};
#pragma pack()


class IMU_YIS100
{
public:
	UART &_com;
	FIFOBuffer<u8, 500> &_Rx_Buffer;
	bool updated;

	IMU_YIS100(UART &com);
	YISData Raw;
	Vector3f Gyro;
	Vector3f Acc;
	Vector3f Angle;
	float Roll;
	float pitch;
	float AngVel;
	float AngAcc;
	bool tagOnLine;
	int updateCnt;
public:
	int8_t Update();
};


#endif
