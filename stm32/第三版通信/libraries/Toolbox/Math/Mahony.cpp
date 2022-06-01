#include "Mahony.h"
#include <math.h>

const float RadianToAngular = 57.2957795f;    //从弧度到角度的转换参数
const float Gyro_Gr = (1/65535) * 4000 * 0.0174532925f; //传感器标度转换参数-> 到弧度


Mahony::Mahony():Quad(new Quaternion(1.0f,0.0f,0.0f,0.0f)){
	q0 = 1.0f;
	q1 = 0.0f;
	q2 = 0.0f;
	q3 = 0.0f;
	exInt = 0.0f;
	eyInt = 0.0f;
	ezInt = 0.0f;
	integralFB.x = integralFB.y = integralFB.z = 0.0f;
}

void Mahony::MahonyAHRSupdate(Vector3f Acc,Vector3f Gyro,Vector3f Mag,float &pitch,float &roll,float &yaw){
	
	float Normal;
	Vector3f Error;
	Vector3f Magent_R,GeoMagent_R;
	Vector3f Gravity,GeoMagent_B;
	
	Gyro *= Gyro_Gr;	
	
	//Acc Normalize
	Normal = invSqrt(Acc.lengthSquared());
	Acc *= Normal;
	
	//Mag Normalize
	Normal = invSqrt(Mag.lengthSquared());
	Mag *= Normal;
	
	Magent_R.x = 2*Mag.x*(0.5-Quad->q2*Quad->q2-Quad->q3*Quad->q3) + 2*Mag.y*(Quad->q1*Quad->q2-Quad->q0*Quad->q3) + 2*Mag.z*(Quad->q1*Quad->q3+Quad->q0*Quad->q2);
	Magent_R.y = 2*Mag.x*(Quad->q1*Quad->q2+Quad->q0*Quad->q3) + 2*Mag.y*(0.5-Quad->q1*Quad->q1-Quad->q3*Quad->q3) + 2*Mag.z*(Quad->q2*Quad->q3-Quad->q0*Quad->q1);	
	Magent_R.z = 2*Mag.x*(Quad->q1*Quad->q3-Quad->q0*Quad->q2) + 2*Mag.y*(Quad->q2*Quad->q3+Quad->q0*Quad->q1) + 2*Mag.z*(0.5-Quad->q1*Quad->q1-Quad->q2*Quad->q2);		
	
	GeoMagent_R.x = sqrt(Magent_R.x*Magent_R.x + Magent_R.y*Magent_R.y);
	GeoMagent_R.z = Magent_R.z;	
	
	GeoMagent_B.x = 2*GeoMagent_R.x*(0.5-Quad->q2*Quad->q2-Quad->q3*Quad->q3)  + 2*GeoMagent_R.z*(Quad->q1*Quad->q3-Quad->q0*Quad->q2);
	GeoMagent_B.y = 2*GeoMagent_R.x*(Quad->q1*Quad->q2-Quad->q0*Quad->q3) + 2*GeoMagent_R.z*(Quad->q2*Quad->q3+Quad->q0*Quad->q1);	
	GeoMagent_B.z = 2*GeoMagent_R.x*(Quad->q0*Quad->q2+Quad->q1*Quad->q3) +  2*GeoMagent_R.z*(0.5-Quad->q1*Quad->q1-Quad->q2*Quad->q2);

//	Magent_R = (*Quad) * Mag;
//	GeoMagent_R.x = sqrt(Magent_R.x*Magent_R.x + Magent_R.y*Magent_R.y);
//	GeoMagent_R.z = Magent_R.z;
//	GeoMagent_B = (*Quad) * GeoMagent_R;
	
	Quad->ToGet_Gravityweight(Gravity.x,Gravity.y,Gravity.z);
	
	Error = Acc % Gravity + Mag % GeoMagent_B;	
	
	integralFB += Error * Ki;

	Gyro += Error * Kp + integralFB; 
	
	Quad->q0 += (-Quad->q1*Gyro.x - Quad->q2*Gyro.y - Quad->q3*Gyro.z)*halfT_;
	Quad->q1 += (Quad->q0*Gyro.x + Quad->q2*Gyro.z - Quad->q3*Gyro.y)*halfT_; 
	Quad->q2 += (Quad->q0*Gyro.y - Quad->q1*Gyro.z + Quad->q3*Gyro.x)*halfT_; 
	Quad->q3 += (Quad->q0*Gyro.z + Quad->q1*Gyro.y - Quad->q2*Gyro.x)*halfT_; 	
	
	Normal = invSqrt(Quad->q0*Quad->q0 + Quad->q1*Quad->q1 + Quad->q2*Quad->q2 + Quad->q3*Quad->q3);
	Quad->q0 *= Normal;
	Quad->q1 *= Normal;
	Quad->q2 *= Normal;
	Quad->q3 *= Normal;
	
	Quad->ToEuler_ZYX(pitch,roll,yaw);
	pitch *= RadianToAngular;
	roll *= RadianToAngular;
	yaw *= RadianToAngular;
	
}

void Mahony::MahonyAHRSupdateIMU(Vector3f Acc,Vector3f Gyro,float &pitch,float &roll,float &yaw){
	Vector3f Gravity,Error;
	float Normal;
	
	
	Gyro *= Gyro_Gr;
	
	if(!((Acc.x==0.0f)&&(Acc.y==0.0f)&&(Acc.z==0.0f))){
		Normal = invSqrt(Acc.lengthSquared());
		Acc *= Normal;
		
		Quad->ToGet_Gravityweight(Gravity.x,Gravity.y,Gravity.z);
		
		Error = Acc % Gravity;	
		
		integralFB += Error * Ki_6;
		
	}
	
	Gyro += Error * Kp_6 + integralFB; 
	
	Quad->q0 += (-Quad->q1*Gyro.x - Quad->q2*Gyro.y - Quad->q3*Gyro.z)*halfT_;
	Quad->q1 += (Quad->q0*Gyro.x + Quad->q2*Gyro.z - Quad->q3*Gyro.y)*halfT_; 
	Quad->q2 += (Quad->q0*Gyro.y - Quad->q1*Gyro.z + Quad->q3*Gyro.x)*halfT_; 
	Quad->q3 += (Quad->q0*Gyro.z + Quad->q1*Gyro.y - Quad->q2*Gyro.x)*halfT_; 	
	
	Normal = invSqrt(Quad->q0*Quad->q0 + Quad->q1*Quad->q1 + Quad->q2*Quad->q2 + Quad->q3*Quad->q3);
	Quad->q0 *= Normal;
	Quad->q1 *= Normal;
	Quad->q2 *= Normal;
	Quad->q3 *= Normal;
	
	Quad->ToEuler_ZYX(pitch,roll,yaw);
	pitch *= RadianToAngular;
	roll *= RadianToAngular;
//	yaw *= RadianToAngular;
	float yaw_G = Gyro.z * RadianToAngular;
	if( (yaw_G > 3.0f ) || (yaw_G < 3.0f)){
		yaw += yaw_G * dt;
	}
}


float Mahony::invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

