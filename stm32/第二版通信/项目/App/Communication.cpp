#include "Communication.h"

PID 					_params;
FLY_DATA			_flydata;




//										FuncCode				TxLen 		RxLen
u8 Protocol[10][3]={
										COM_PID_PARAMS  ,   36   ,		36	,
										COM_FLY_DATA		,		53	 ,		1		,
					};

				
int Communication::CheckData(){
	int fnCode =_UIDLink.Pack_Check();
	if(fnCode>=0){
		switch(fnCode){
			case COM_PID_PARAMS:
				Control::Param_PitchLoop_Kp = (float)(_UIDLink.GetSigned16(0)) / 1000;//½Ç¶È
				Control::Param_PitchLoop_Ki = (float)(_UIDLink.GetSigned16(2) )/ 1000;
				Control::Param_PitchLoop_Kd = (float)(_UIDLink.GetSigned16(4) )/ 1000;
				Control::Param_RollLoop_Kp = (float)(_UIDLink.GetSigned16(6) )/ 1000;
				Control::Param_RollLoop_Ki = (float)(_UIDLink.GetSigned16(8) )/ 1000;
				Control::Param_RollLoop_Kd = (float)(_UIDLink.GetSigned16(10) )/ 1000;
				Control::Param_YawLoop_Kp = (float)(_UIDLink.GetSigned16(12) )/ 1000;
				Control::Param_YawLoop_Ki = (float)(_UIDLink.GetSigned16(14) )/ 1000;
				Control::Param_YawLoop_Kd = (float)(_UIDLink.GetSigned16(16) )/ 1000;
				Control::Param_PitchVelLoop_Kp = (float)(_UIDLink.GetSigned16(18)) / 1000;//½ÇËÙ¶È
				Control::Param_PitchVelLoop_Ki = (float)(_UIDLink.GetSigned16(20) )/ 1000;
				Control::Param_PitchVelLoop_Kd = (float)(_UIDLink.GetSigned16(22) )/ 1000;
				Control::Param_RollVelLoop_Kp = (float)(_UIDLink.GetSigned16(24) )/ 1000;
				Control::Param_RollVelLoop_Ki = (float)(_UIDLink.GetSigned16(26) )/ 1000;
				Control::Param_RollVelLoop_Kd = (float)(_UIDLink.GetSigned16(28) )/ 1000;
				Control::Param_YawVelLoop_Kp = (float)(_UIDLink.GetSigned16(30) )/ 1000;
				Control::Param_YawVelLoop_Ki = (float)(_UIDLink.GetSigned16(32) )/ 1000;
				Control::Param_YawVelLoop_Kd = (float)(_UIDLink.GetSigned16(34) )/ 1000;
				break;	
			case COM_FLY_DATA:
				_fly_state = _UIDLink.GetUnSigned8(0);break;
		}	
	}
	return fnCode;
}

void Communication::SendData(int FnCode){
	static uint16_t Sequence_FLYDATA  = 0;
	switch(FnCode){
		case COM_PID_PARAMS:	
			_params.pid_param[0] = (int16_t)(Control::Param_PitchLoop_Kp * 1000);
			_params.pid_param[1] = (int16_t)(Control::Param_PitchLoop_Ki * 1000);
			_params.pid_param[2] = (int16_t)(Control::Param_PitchLoop_Kd * 1000);
			_params.pid_param[3] = (int16_t)(Control::Param_RollLoop_Kp * 1000);
			_params.pid_param[4] = (int16_t)(Control::Param_RollLoop_Ki * 1000);
			_params.pid_param[5] = (int16_t)(Control::Param_RollLoop_Kd * 1000);
			_params.pid_param[6] = (int16_t)(Control::Param_YawLoop_Kp * 1000);
			_params.pid_param[7] = (int16_t)(Control::Param_YawLoop_Ki * 1000);
			_params.pid_param[8] = (int16_t)(Control::Param_YawLoop_Kd * 1000);
			_params.pid_param[9] = (int16_t)(Control::Param_PitchVelLoop_Kp * 1000);
			_params.pid_param[10] = (int16_t)(Control::Param_PitchVelLoop_Ki * 1000);
			_params.pid_param[11] = (int16_t)(Control::Param_PitchVelLoop_Kd * 1000);
			_params.pid_param[12] = (int16_t)(Control::Param_RollVelLoop_Kp * 1000);
			_params.pid_param[13] = (int16_t)(Control::Param_RollVelLoop_Ki * 1000);
			_params.pid_param[14] = (int16_t)(Control::Param_RollVelLoop_Kd * 1000);
			_params.pid_param[15] = (int16_t)(Control::Param_YawVelLoop_Kp * 1000);
			_params.pid_param[16] = (int16_t)(Control::Param_YawVelLoop_Ki * 1000);
			_params.pid_param[17] = (int16_t)(Control::Param_YawVelLoop_Kd * 1000);
			_UIDLink._Tx_Frame._FunctionCode = COM_PID_PARAMS;
			_UIDLink.CopyFromTxData(_params);
			_UIDLink.Pack_Send();
			break;
		case COM_FLY_DATA:
			_flydata.Seq = Sequence_FLYDATA++;
			_flydata.roll = (int16_t)(roll*100);
			_flydata.pitch = (int16_t)(pitch*100);
			_flydata.yaw = (int16_t)(yaw*100);
			_flydata.Acc_x = Acc.x;
			_flydata.Acc_y = Acc.y;
			_flydata.Acc_z = Acc.z;
			_flydata.Gyro_x = Gyro.x;
			_flydata.Gyro_y = Gyro.y;
			_flydata.Gyro_z = Gyro.z;
			_flydata.Mag_x = Mag.x;
			_flydata.Mag_y = Mag.y;
			_flydata.Mag_z = Mag.z;
			_flydata.Press = press;
			_flydata.optical_flow_x = 0;
			_flydata.optical_flow_y = 0;
			_flydata.laser_distance = 0;
			for(u8 i=0;i<9;i++)
				_flydata.Rc_Channel[i] = _sbus.Channel_Origin[i];
			_flydata.Fly_state = 0;
			
			_UIDLink._Tx_Frame._FunctionCode = COM_FLY_DATA;
			_UIDLink.CopyFromTxData(_flydata);
			_UIDLink.Pack_Send();
			break;
			
	}
}

int Communication::Updata(){
	static int i=0;
	i++;
	if(i==1)Communication::CheckData();
	if(i==2)Communication::SendData(COM_PID_PARAMS);
	if(i==3)Communication::SendData(COM_FLY_DATA);
	
	if(i==3)i=0;
	return 0;
	
}

