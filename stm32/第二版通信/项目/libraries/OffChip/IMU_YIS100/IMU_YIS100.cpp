#include "IMU_YIS100.h"


IMU_YIS100::IMU_YIS100(UART &com)
:_com(com),_Rx_Buffer(com._RX_Buffer)
{
	Roll = 1;
	AngVel = 1;
	pitch = 1;
	AngAcc = 1;
	tagOnLine = false;
	updated = false;
	updateCnt = 1;
}

//#include "GPIO.h"

//extern GPIO LED1;

int8_t IMU_YIS100::Update()
{
	static int maxCNT = 10;
	updateCnt++;
	if(updateCnt>maxCNT)
	{
		updateCnt = maxCNT + 10;
		tagOnLine = false;
	}
	updated = false;
	
	//Header byte 1
	while(_Rx_Buffer._size_>=49 && (_Rx_Buffer._buffer[_Rx_Buffer._index_out]!= 0x59)) //find frame header
	{
		if(++_Rx_Buffer._index_out >= _Rx_Buffer._max_size_)		_Rx_Buffer._index_out = 0;
		_Rx_Buffer._size_--;
	}
	//Header byte 2
	u16 idx = _Rx_Buffer._index_out + 1;
	if(idx>=_Rx_Buffer._max_size_) idx -= _Rx_Buffer._max_size_;   //BID index
	
	u8 header2 = _Rx_Buffer._buffer[idx];                        //get BID code
	
	if(header2!=0x53)
	{
		_Rx_Buffer.GetData(header2);
		return -1;
	}
		
	if(_Rx_Buffer._size_<49)
		return -2;
	

	//read data
	_Rx_Buffer.GetDatas((u8*)&Raw,49);
	
	//eck sum
	u8* p = (u8*)&Raw;
	u8 ck1=0, ck2=0;
	for(int i=2; i<47;i++)
	{
	  ck1 += p[i];
		ck2 += ck1;
	}
	if(Raw.sum1!=ck1 || Raw.sum2!=ck2)  //eck sum failed
		return -3;
	
	//eck data id
	if(Raw.accID!=0x10 || Raw.gyroID!=0x20 || Raw.angID!=0x40)
		return -4;
	
	//eck data len
	if(Raw.LEN!=0x2A || Raw.accLen!=0x0C || Raw.gyroLen!=0x0C || Raw.angLen!=0x0C)
		return -5;            //eck data len failed
	
	
	
	
	Acc.x = Raw.accx * 0.000001f;
	Acc.y = Raw.accy * 0.000001f;
	Acc.z = Raw.accz * 0.000001f;
	
	Gyro.x = Raw.gyrox * 0.000001f;
	Gyro.y = Raw.gyroy * 0.000001f;
	Gyro.z = Raw.gyroz * 0.000001f;
	
	Angle.x = Raw.pitch * 0.000001f;
	Angle.y = Raw.roll * 0.000001f;
	Angle.z = Raw.yaw * 0.000001f;
	
	Roll = Roll + 0.99f*(Angle.y - Roll);
	AngAcc = AngAcc + 0.99f*((Gyro.x - AngVel)/0.02f - AngAcc);
	AngVel = AngVel + 0.99f*(Gyro.x - AngVel);
	
	
	pitch = pitch + 0.99f*(Angle.x - pitch);
	//AngAcc = AngAcc + 0.99f*((Gyro.x - AngVel)/0.02f - AngAcc);
	//AngVel = AngVel + 0.99f*(Gyro.x - AngVel);
	
	updated = true;
	tagOnLine = true;
	updateCnt = 1;
	
	if(_Rx_Buffer._size_>=49)
		Update();
	
	return 0;
}
