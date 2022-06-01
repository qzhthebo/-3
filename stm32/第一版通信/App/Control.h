#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"
#include "RemoteControl.h"
#include "DCBLMotor.h"
#include "MPU6050.h"
#include "SBUS.h"
#include "State.h"
#define Gyro_Gr (1/65535) * 4000 * 0.0174532925f
#define ABS(x)	 ( (x)>0?(x):-(x) )
#define LIMIT(x,min,max) ( (x)<(min)? (min): ((x)>(max)? (max):(x)) )
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b)) 

#define InLoop_Debug 1 

#define CTRL_MAX_ANGELVEL_ASPEED			300.0f		//pitch,roll�����������ƽ��ٶ�
#define CTRL_MAX_Yaw_SPEED						150.0f		//yaw�����������ƽ��ٶ�
#define CTRL_INLOOP_LIMIT							1.0f*CTRL_MAX_ANGELVEL_ASPEED		//�ڻ����ַ���

#define CTRL_MAX_ANGLE								45.0f			//ң���ܿ��Ƶ����Ƕ�
#define ANGLE_TO_MAX_AS								45.0f			//�Ƕ����ΪNʱ,�������ٶȴﵽ���(Ҳ����ͨ���ڻ�P����)
#define CTRL_OUTLOOP_LIMIT						0.5f*CTRL_MAX_ANGLE								//�⻷���ַ���

#define RadtoDeg  57.2957795f
#define DegtoRad  0.00174532f
#define PI				3.1415926f

#define MAX_PWM 	100							//���PWM���100%����
#define MAT_THR		80						//����ͨ�����ռ��80%,��20%��������

//===============================   Global Params ==============================//

extern SBUS _sbus;
extern DCBLMotor _M1;
extern DCBLMotor _M2;
extern DCBLMotor _M3;
extern DCBLMotor _M4;

extern RemoteControl _remote;
extern Vector3f Acc,Gyro,Mag;
extern float press;
extern float pitch,roll,yaw;
extern float altitude;
extern u16 ROLL_P,PITCH_P,YAW_P,THROLLTE_P;

class Control{
public:
		//Control Target
		static float Roll_Target;
		static float Pitch_Target;
		static float Yaw_Target;
		static float Throttle_Target;	


		//Control Output Values for Angle Loop Angular Loop
		static float CTRL_Pitch_I_Out;
		static float CTRL_Roll_I_Out;
		static float CTRL_Yaw_I_Out;
		static float CTRL_Pitch_D_Out;
		static float CTRL_Roll_D_Out;
		static float CTRL_Yaw_D_Out;
		static float CTRL_Pitch_Out;
		static float CTRL_Roll_Out;
		static float CTRL_Yaw_Out;
		static float CTRL_PitchVel_I_Out;
		static float CTRL_RollVel_I_Out;
		static float CTRL_YawVel_I_Out;
		static float CTRL_PitchVel_D_Out;
		static float CTRL_RollVel_D_Out;
		static float CTRL_YawVel_D_Out;		
		static float CTRL_PitchVel_Out;
		static float CTRL_RollVel_Out;
		static float CTRL_YawVel_Out;		
		//Control Parameters for Angle Loop , Angular Loop
		static float Param_RollLoop_Kp;
		static float Param_RollLoop_Ki;
		static float Param_RollLoop_Kd;
		static float Param_PitchLoop_Kp;
		static float Param_PitchLoop_Ki;
		static float Param_PitchLoop_Kd;
		static float Param_YawLoop_Kp;
		static float Param_YawLoop_Ki;
		static float Param_YawLoop_Kd;
		static float Param_RollVelLoop_Kp;
		static float Param_RollVelLoop_Ki;
		static float Param_RollVelLoop_Kd;
		static float Param_PitchVelLoop_Kp;
		static float Param_PitchVelLoop_Ki;
		static float Param_PitchVelLoop_Kd;
		static float Param_YawVelLoop_Kp;
		static float Param_YawVelLoop_Ki;
		static float Param_YawVelLoop_Kd;
		static float pwm1;
		static float pwm2;
		static float pwm3;
		static float pwm4;

public:

		static void normal_control();
		static void PitchVelLoop_PIDControl(float T);
    static void PitchLoop_PIDControl(float T);
		static void Update();
		static void ToPWM();
};
#endif
