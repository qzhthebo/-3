#ifndef _MAHONY_H_
#define _MAHONY_H_

#include "MathTools.h"
#include "UART.h"


#define Kp 10.0f                   //9��Kp����100����Ʈ    ��ѧ����         
#define Ki 0.003f        		//����������,���������������������ٶ�  

#define Kp_6 18.0f                   //6��Kp����100����Ʈ           
#define Ki_6 0.003f        		//����������,���������������������ٶ�   

#define halfT_ 0.0005f  
#define dt 0.001f 
#define IMU_INTEGRAL_LIM  0.034906585f   //�����޷�


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
