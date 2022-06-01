#ifndef _REMOTECONTROL_H_
#define _REMOTECONTROL_H_

#include "stm32f10x.h"
#include "SBUS.h"

/*
	SBUS ԭʼֵ����SBUS_Max SBUS_Min
	->	SBUSͨ���ٷֱ� 0  100
	->	PWMͨ�����ްٷֱ� 1100 1900
*/


/*
Pitch 	Max ��  Min ��
Yaw 		Max ��  Min ��
Rol			Max ��	Min ��
Thr			Max	��	Min ��
*/

#define REMOTECONTROL_LOSE 0x00
#define REMOTECONTROL_LOCK 0x10
#define REMOTECONTROL_UNLOCK 0x11
#define REMOTECONTROL_CALIB 0x12

#define RightLow_LeftLow  1	//�ڰ���
#define LeftLow_RightLow  2	//�����
#define LeftLow_RightUp  	3 //����+����
#define LeftLow_LeftLow   4 //����+����
#define RightUp_RightUp 	5 //����+����
#define LeftLow_All  			6 //����+����
#define RightLow_All      7 //����+����
#define Up_Low			  		8 //��+��

#define R9DS_ORIGINAL_MAX 1900
#define R9DS_ORIGINAL_MIN 1100



class RemoteControl{
private:
	SBUS &_Cap;



//Mode Params and Selection
	u8  LockPos;				
	u8  UnlockPos;				
	u8  LostPos;    			
	u8	CalibrationPos;		
	u8 StateSelection(u8 state);

public:
	RemoteControl(SBUS &_CAP);
	~RemoteControl();

	u8 temp;
	u8	State;
//�ɱ�����
	uint16_t PITMAX;
	uint16_t PITMIN;
	uint16_t ROLMAX;
	uint16_t ROLMIN;
	uint16_t THRMAX;
	uint16_t THRMIN;
	uint16_t YAWMAX;
	uint16_t YAWMIN;

	u8 pwm1;
	u8 pwm2;
	u8 pwm3;
	u8 pwm4;


//Output As PWM*1000
	uint16_t mPITCH;
	uint16_t mTHROTTLE;
	uint16_t mYAW;
	uint16_t mROLL;
	uint16_t mFlihtmode;
	uint16_t mSightmode;
	uint16_t mUpsidewave;
	uint16_t mUpsidelean;


	u8 Update(u16 time_ms,u16 Time_Of_Duration_ms);

//Turn to Percent to PWM Value
	void TurntoPWM();
//Modify the Remote Control Mode
	void SetLock(u8 Choose);
	void SetLost(u8 Choose);
	void SetUnLock(u8 Choose);
	void SetCalibration(u8 Choose);
//Enter the Calibration Mode
	void StartCalibration();
	void EndCalibration();
	void DebugPWM();
};














#endif
