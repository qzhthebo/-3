#ifndef _MAHONY_H_
#define _MAHONY_H_

#include "MathTools.h"
#include "UART.h"


#define Kp 10.0f                   //9轴Kp超过100后乱飘    玄学调参         
#define Ki 0.003f        		//误差积分增益,决定了陀螺仪误差的收敛速度  

#define Kp_6 18.0f                   //6轴Kp超过100后乱飘           
#define Ki_6 0.003f        		//误差积分增益,决定了陀螺仪误差的收敛速度   

#define halfT_ 0.0005f  
#define dt 0.001f 
#define IMU_INTEGRAL_LIM  0.034906585f   //积分限幅


class Mahony{
public:
		float q0,q1,q2,q3;
		float exInt,eyInt,ezInt; 

		Vector3f integralFB;
		Quaternion *Quad;
public:
	Mahony();
	float invSqrt(float x);
	void MahonyAHRSupdate(Vector3f Acc,Vector3f Gyro,Vector3f Mag,float &pitch,float &roll,float &yaw);
	void MahonyAHRSupdateIMU(Vector3f Acc,Vector3f Gyro,float &pitch,float &roll,float &yaw);

};




#endif
