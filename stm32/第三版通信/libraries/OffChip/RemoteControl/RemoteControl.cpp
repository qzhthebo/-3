#include "RemoteControl.h"


RemoteControl::RemoteControl(SBUS &_CAP):_Cap(_CAP){
	
	State = REMOTECONTROL_LOCK;					//默认锁定
	
	temp = 0;
	
	PITMAX = R9DS_ORIGINAL_MAX;
	PITMIN = R9DS_ORIGINAL_MIN;
	
	ROLMAX = R9DS_ORIGINAL_MAX;
	ROLMIN = R9DS_ORIGINAL_MIN;
	
	THRMAX = R9DS_ORIGINAL_MAX;
	THRMIN = R9DS_ORIGINAL_MIN;
	
	YAWMAX = R9DS_ORIGINAL_MAX;
	YAWMIN = R9DS_ORIGINAL_MIN;
	
	
	LockPos = RightLow_LeftLow;			//内八字 
	UnlockPos = LeftLow_RightLow;		//外八字
	LostPos = LeftLow_LeftLow;        	//左下左下
	CalibrationPos = LeftLow_RightUp; //左下+右上
}

RemoteControl::~RemoteControl(){
}

u8 RemoteControl::Update(u16 time_ms,u16 Time_Of_Duration_ms){
	
	DebugPWM();
	static u16 Lock_cnt=0,Unlock_cnt=0,Calibration_cnt=0;
	u16 cnt=(u16)(Time_Of_Duration_ms/time_ms);

	_Cap.Update();
	
	TurntoPWM();
	
//	if(State == REMOTECONTROL_CALIB){
//		temp=1;
//		StartCalibration();
//	}
	if(StateSelection(LockPos)){
		Lock_cnt++;
		Unlock_cnt = 0;
		Calibration_cnt=0;
		temp=cnt;
		if(cnt == Lock_cnt){
			temp=20;
			Lock_cnt = 0;
			//if(State!=REMOTECONTROL_CALIB)
				State = REMOTECONTROL_LOCK;
			return State;
		}else
			return State;
	}
	else if(StateSelection(UnlockPos)){
		Lock_cnt = 0;
		Unlock_cnt++;
		Calibration_cnt=0;
		temp=3;
		if(cnt == Unlock_cnt){
			temp=30;
			Unlock_cnt = 0;
			if(State!=REMOTECONTROL_CALIB)
				State = REMOTECONTROL_UNLOCK;
			return State;
		}else
			return State;		
	}
	else if(StateSelection(CalibrationPos)){
		Lock_cnt = 0;
		Unlock_cnt = 0;
		Calibration_cnt++;
		temp=4;
		if(cnt >= Calibration_cnt){
			temp=40;
			Calibration_cnt = 0;
			if(State==REMOTECONTROL_LOCK)
				State = REMOTECONTROL_CALIB;
			return State;
		}else
			return State;			
	}
	else if(StateSelection(LostPos)){
		Lock_cnt = 0;
		Unlock_cnt = 0;
		Calibration_cnt = 0;
		temp=5;
		if(State != REMOTECONTROL_CALIB)
			State = REMOTECONTROL_LOSE;
		return State;
	}else{
		temp=6;
		Lock_cnt = 0;
		Unlock_cnt = 0;
		Calibration_cnt = 0;
		State = State | 0x10;
		return State;
	}
}


u8 RemoteControl::StateSelection(u8 state){
	static u16 ThroMin;//,ThroMax;
	static u16 YawMin,YawMax;
	static u16 PitMin,PitMax;
	static u16 RolMin,RolMax;
	u16 sbusmax = 1500,sbusmin = 400;
	
	ThroMin = (_Cap.Channel_Origin[RC_THROL] <= sbusmin);
//	ThroMax = (_Cap.Channel_Origin[RC_THROL] <= sbusmax);
	
	YawMin = (_Cap.Channel_Origin[RC_YAW] <= sbusmin);
	YawMax = (_Cap.Channel_Origin[RC_YAW] >= sbusmax);
	
	PitMin = (_Cap.Channel_Origin[RC_PITCH] <= sbusmin);
	PitMax = (_Cap.Channel_Origin[RC_PITCH] >= sbusmax);
	
	RolMin = (_Cap.Channel_Origin[RC_ROLL] <= sbusmin);
	RolMax = (_Cap.Channel_Origin[RC_ROLL] >= sbusmax);
	
	switch(state){
		case RightLow_LeftLow:
			if(ThroMin && YawMax && PitMax && RolMin)
				return 1;
			else 
				return 0;
		case LeftLow_RightLow:
			if(ThroMin && YawMin && PitMax && RolMax)
				return 1;
			else 
				return 0;
		case LeftLow_RightUp:
			if(ThroMin && YawMin && PitMin && RolMax)
				return 1;
			else 
				return 0;
		case LeftLow_LeftLow:
			if(ThroMin && YawMin && PitMax && RolMin)
				return 1;
			else 
				return 0;
		case RightUp_RightUp:
			if(ThroMin && YawMax && PitMax && RolMax)
				return 1;
			else 
				return 0;
		case LeftLow_All:
			if(ThroMin && YawMin)
				return 1;
			else 
				return 0;
		case RightLow_All:
			if(ThroMin && YawMax)
				return 1;
			else 
				return 0;
		case Up_Low:
			if(ThroMin && PitMin)
				return 1;
			else 
				return 0;
		default:
			return 0;
	}
		
}
void RemoteControl::TurntoPWM(){
//	mPITCH 		= PITMIN + (_Cap._PITCH_ 	* (PITMAX-PITMIN))/100;
//	mROLL  		= ROLMIN + (_Cap._ROLL_   * (ROLMAX-ROLMIN))/100;
//	mTHROTTLE = THRMIN + (_Cap._THROL_  * (THRMAX-THRMIN))/100;
//	mYAW 			= YAWMIN + (_Cap._YAW_ 		* (YAWMAX-YAWMIN))/100;
	
	mPITCH = _Cap._PITCH_ ;
	mROLL	 = _Cap._ROLL_ ;
	mTHROTTLE = _Cap._THROL_;
	mYAW = _Cap._YAW_ ;
	
	
	mFlihtmode = _Cap._FLIGHTMODE_;
	mSightmode = _Cap._SIGHTMODE_;
	
	mUpsidewave = R9DS_ORIGINAL_MIN + (_Cap._UPSIDE_WAVE_ * (R9DS_ORIGINAL_MAX-R9DS_ORIGINAL_MIN))/100.0f;
	mUpsidelean = R9DS_ORIGINAL_MIN + (_Cap._UPSIDE_LEAN_ * (R9DS_ORIGINAL_MAX-R9DS_ORIGINAL_MIN))/100.0f;
	
}


void RemoteControl::DebugPWM(){
	pwm1 = (_Cap.Channel[0]-49)*8/50+11;
	pwm2 = (_Cap.Channel[1]-49)*8/50+11;
	pwm3 = (_Cap.Channel[2])*8/100+11;
	pwm4 = (_Cap.Channel[3]-49)*8/50+11;
}

