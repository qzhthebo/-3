#include "Control.h"
#include "State.h"

float Control::CTRL_Pitch_I_Out = 0.0;
float Control::CTRL_Roll_I_Out = 0.0;
float Control::CTRL_Yaw_I_Out = 0.0;
float Control::CTRL_Pitch_D_Out = 0.0;
float Control::CTRL_Roll_D_Out = 0.0;
float Control::CTRL_Yaw_D_Out = 0.0;
float Control::CTRL_Pitch_Out = 0.0;
float Control::CTRL_Roll_Out = 0.0;
float Control::CTRL_Yaw_Out = 0.0;
float Control::CTRL_PitchVel_I_Out = 0.0;
float Control::CTRL_RollVel_I_Out = 0.0;
float Control::CTRL_YawVel_I_Out = 0.0;
float Control::CTRL_PitchVel_D_Out = 0.0;
float Control::CTRL_RollVel_D_Out = 0.0;
float Control::CTRL_YawVel_D_Out = 0.0;
float Control::CTRL_PitchVel_Out = 0.0;
float Control::CTRL_RollVel_Out = 0.0;
float Control::CTRL_YawVel_Out = 0.0;

//================================== Pid Control Params , Should Set by User
float Control::Param_RollLoop_Kp = 0.008;
float Control::Param_RollLoop_Ki = 0.001;
float Control::Param_RollLoop_Kd = 1;


float Control::Param_PitchLoop_Kp = 0.03;
float Control::Param_PitchLoop_Ki = 0.05;
float Control::Param_PitchLoop_Kd = 0.001;



float Control::Param_YawLoop_Kp = 0.00;
float Control::Param_YawLoop_Ki = 0.05 ;
float Control::Param_YawLoop_Kd = 0.5;
float Control::Param_RollVelLoop_Kp = 0.8;
float Control::Param_RollVelLoop_Ki = 0.1 ;
float Control::Param_RollVelLoop_Kd = 2 ;


float Control::Param_PitchVelLoop_Kp = 0.46;
float Control::Param_PitchVelLoop_Ki = 0.0;
float Control::Param_PitchVelLoop_Kd = 0.035;



float Control::Param_YawVelLoop_Kp = 1.2;
float Control::Param_YawVelLoop_Ki = 1 ;
float Control::Param_YawVelLoop_Kd = 1 ;

float Control::Roll_Target = 0.0f;
float Control::Pitch_Target = 0.0f;
float Control::Yaw_Target = 0.0f;
float Control::Throttle_Target = 0.0f;

float Control::pwm1=0;
float Control::pwm2=0;
float Control::pwm3=0;
float Control::pwm4=0;


float trans(float m){
	return LIMIT(m*0.08+11.0,11.0,19.0);
}



void Control::PitchVelLoop_PIDControl(float T){
	
	float PitchVel_E = 0;
	
  float PitchVelTarget = CTRL_Pitch_Out*7;
	
	//float PitchVelTarget = 180;
	
	static float Gyroy_last = 0;
		
	PitchVel_E = PitchVelTarget - Gyro.y;                                            //p
	
	PitchVel_E = LIMIT(PitchVel_E , -300 , 300);                
	
	CTRL_PitchVel_D_Out = Param_PitchVelLoop_Kd * (PitchVel_E -  Gyroy_last) / T;   //D
	
	CTRL_PitchVel_I_Out += PitchVel_E;
	
	CTRL_PitchVel_Out = PitchVel_E * Param_PitchVelLoop_Kp  + CTRL_PitchVel_D_Out ;//+ CTRL_PitchVel_I_Out*Param_PitchVelLoop_Ki;
	
	CTRL_PitchVel_Out = LIMIT(CTRL_PitchVel_Out , -300 , 300);
	
	Gyroy_last = PitchVel_E;
}




void Control::PitchLoop_PIDControl(float T){
	
	
	float Pitch_E = 0;
	
	static float Pitch_E_Last = 0; 
	Pitch_Target = Param_YawLoop_Kp;
	Pitch_E = Pitch_Target - pitch;	
	Pitch_E = LIMIT(Pitch_E , -50 , 50);        
	 
	CTRL_Pitch_D_Out = Param_PitchLoop_Kd * (pitch - Pitch_E_Last)/T;          //D
	
	CTRL_Pitch_Out = Param_PitchLoop_Kp * Pitch_E + CTRL_Pitch_D_Out;
	
	CTRL_Pitch_Out = LIMIT(CTRL_Pitch_Out , -300 , 300);
	
	Pitch_E_Last = pitch;
	
}



//void Control::RollLoop_PIDControl(float T){
//	float Roll_E = 0;
//	static float Roll_E_Last = 0;
//	
//	Roll_E = Roll_Target - roll;                                             //P
//	Roll_E = LIMIT(Roll_E , -45 , 45);
//	
//	CTRL_Roll_D_Out = Param_RollLoop_Kd * (roll - Roll_E_Last)/T;             //D
//	
//	CTRL_Roll_Out = Param_RollLoop_Kp * Roll_E + CTRL_Roll_D_Out;	
//	
//	CTRL_Roll_Out = LIMIT(CTRL_Roll_Out , -300 , 300);
//	
//	Roll_E_Last = roll;
//}



//void Control::YawLoop_PIDControl(float T){
//	float Yaw_E = 0;
//	static float Yaw_E_Last = 0;
//	
//	Yaw_E = Yaw_Target - yaw;
//	Yaw_E = LIMIT(Yaw_E , -45 , 45);
//	
//	CTRL_Yaw_D_Out = Param_YawLoop_Kd * (yaw - Yaw_E_Last)/T;                //D
//	
//	CTRL_Yaw_Out = Param_YawLoop_Kp * Yaw_E + CTRL_Yaw_D_Out;	

//	CTRL_Yaw_Out = LIMIT(CTRL_Yaw_Out , -300 , 300);	 
//	
//	Yaw_E_Last = yaw;
//}



//void Control::PitchVelLoop_PIDControl(float T){
//	float PitchVel_E = 0;
//	
//	float PitchVelTarget = (Pitch_Target-pitch)*4;
//		
//	static float PitchVel_E_Last = 0;
//		
//	PitchVel_E = PitchVelTarget - Gyro.y;                                            //p
//	
//	PitchVel_E = LIMIT(PitchVel_E , -300 , 300);                
//	
//	CTRL_PitchVel_D_Out = Param_PitchVelLoop_Kd * (Gyro.y -  PitchVel_E_Last) / T;   //D
//	
//	CTRL_PitchVel_Out = PitchVel_E * Param_PitchVelLoop_Kp + CTRL_PitchVel_D_Out;
//	
//	CTRL_PitchVel_Out = LIMIT(CTRL_PitchVel_Out , -300 , 300);
//	
//	PitchVel_E_Last = Gyro.y;
//}



//void Control::RollVelLoop_PIDControl(float T){
//	
//	float RollVel_E = 0;
//	
//	float RollVelTarget = 0;
//	
//	static float RollVel_E_Last = 0;

//	RollVel_E = RollVelTarget - Gyro.x;                                             //p
//	
//	RollVel_E = LIMIT(RollVel_E , -300 , 300);	
//	
//	CTRL_RollVel_D_Out = Param_RollVelLoop_Kd * (Gyro.x - RollVel_E_Last) / T;       //D
//	
//	CTRL_RollVel_Out = Param_RollVelLoop_Kp * RollVel_E+CTRL_RollVel_D_Out;	
//	
//	CTRL_RollVel_Out = LIMIT(CTRL_RollVel_Out , -300 , 300);
//	
//	RollVel_E_Last = Gyro.x;
//}



//void Control::YawVelLoop_PIDControl(float T){
////	float YawVel_E = 0;
////	static float YawVel_E_Last = 0;
////	//角度控制输出到角速度期望的比例映射
////	float YawVelTarget =  LIMIT(CTRL_MAX_ANGELVEL_ASPEED*(CTRL_Yaw_Out/ANGLE_TO_MAX_AS)
////		,-CTRL_MAX_ANGELVEL_ASPEED,CTRL_MAX_ANGELVEL_ASPEED);
////	
////	YawVel_E = YawVelTarget - Gyro.x;
////	YawVel_E_Last = LIMIT(YawVel_E_Last , -300 , 300);	
////	
////	//I
////	CTRL_YawVel_I_Out += Param_YawVelLoop_Ki * (YawVel_E)*T;
////	//I LIMIT
////	CTRL_YawVel_I_Out = LIMIT(CTRL_YawVel_I_Out,-CTRL_INLOOP_LIMIT,CTRL_INLOOP_LIMIT);
////	//D
////	CTRL_YawVel_D_Out = Param_YawVelLoop_Kd*(YawVel_E-YawVel_E_Last)/T;
////	CTRL_YawVel_Out = Param_YawVelLoop_Kp*(YawVel_E+CTRL_YawVel_I_Out+CTRL_YawVel_D_Out);
////	
////	YawVel_E_Last = YawVel_E;
//}







void Control::ToPWM(){
	Control::pwm1 = trans(_sbus._THROL_+(CTRL_PitchVel_Out)*100.0/300.0);
	Control::pwm2 = trans(_sbus._THROL_-(CTRL_PitchVel_Out)*100.0/300.0);
	Control::pwm3 = trans(_sbus._THROL_-(CTRL_PitchVel_Out)*100.0/300.0);
	Control::pwm4 = trans(_sbus._THROL_+(CTRL_PitchVel_Out)*100.0/300.0);
}

//void Control::ToPWM(){
//	Control::pwm1 = trans(35+(CTRL_PitchVel_Out)*65.0/300.0);
//	Control::pwm2 = trans(35-(CTRL_PitchVel_Out)*65.0/300.0);
//	Control::pwm3 = trans(35-(CTRL_PitchVel_Out)*65.0/300.0);
//	Control::pwm4 = trans(35+(CTRL_PitchVel_Out)*65.0/300.0);
//}

void debug_control(){
			_remote.DebugPWM();
			_M1.SetThrottle(_remote.pwm1);
			_M2.SetThrottle(_remote.pwm2);
			_M3.SetThrottle(_remote.pwm3);
			_M4.SetThrottle(_remote.pwm4);	
}

void Control::normal_control(){
	Control::PitchLoop_PIDControl(0.01);
	Control::PitchVelLoop_PIDControl(0.01);
	Control::ToPWM();
	_M1.SetThrottle(pwm1);
	_M2.SetThrottle(pwm2);
	_M3.SetThrottle(pwm3);
	_M4.SetThrottle(pwm4);		
}

	
void stop_control(){
	_M1.SetThrottle(11);
	_M2.SetThrottle(11);
	_M3.SetThrottle(11);
	_M4.SetThrottle(11);
}
	


void control()
{

	switch(_sbus._FLIGHTMODE_){
		case DEBUG:
			debug_control();
			break;
		
		case NORMAL:
			Control::normal_control();
			break;
		
		case STOP:
			stop_control();
			break;
	}
}


void Control::Update(){

	switch(State::state){
		case INIT:
		stop_control();
			break;

		case STANDBY:
			stop_control();
			break;
		
		case RUNNING:
			control();
			break;	
		
		case ERROR:
			stop_control();
			break;	
}
}























