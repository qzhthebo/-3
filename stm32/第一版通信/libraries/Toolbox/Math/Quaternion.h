#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "math.h"
#include "Vector3.h"


/*
==========================     Matrix for Pos Quaternion   ===========================
	刚体固联系b
	参考坐标系(地球)R
					[ 1-2(q2^2+q3^2)			2(q1q2-q0q3)			2(q1q3+q0q2) ]
C(b_R)  =	[		2(q1q2+q0q3)		1-2(q1^2+q3^2)			2(q2q3-q0q1) ]
					[		2(q1q3-q0q2)			2(q2q3+q0q1)     1-2(q1^2+q2^2)]

C(R_b)	即C(b_R) 的转置

==========================     Matrix for Pos Euler   ===========================
	pitch -> θ
	roll	-> γ
	yaw   -> ψ
					[ cosψcosθ			sinψcosγ+sinγsinθcosψ			sinψsinγ-sinθcosψcosγ ]
C(b_R)  =	[	-sinψcosθ			cosψcosγ-sinθsinψsinγ			cosψsinγ+sinθsinψcosγ ]
					[		sinθ							-cosθsinγ     						cosγcosθ				]

Euler:
	pitch = arcsin(M31)
	roll  = arctan(-M32/M33)
	yaw		= arctan(-M21/M11)

*/




class Quaternion{
public:
	float q0,q1,q2,q3;
public:
	//========================== Constructor =========================//
	Quaternion(){
		q0=1;q1=q2=q3=0;
	}
	Quaternion(float _q0,float _q1,float _q2,float _q3)
	:q0(_q0),q1(_q1),q2(_q2),q3(_q3){}
	//========================== Constructor =========================//

	//value call
	void operator()(float _q0,float _q1,float _q2,float _q3){
		q0=_q0; q1=_q1; q2=_q2; q3=_q3;
	}

	//Check element is number or not
	bool IsNan(void){
		return isnan(q0) || isnan(q1) || isnan(q2) || isnan(q3); 
	}

	//To Eulers Base on Attitude Quaternion Matrix
	//Z-Y-X order
	void ToEuler_ZYX(float &pitch,float &roll,float &yaw){
		roll = atan2(2.0*(q0*q1+q2*q3) , 1-2.0*(q1*q1+q2*q2));
		pitch = asin(-2.0*(q0*q2-q1*q3));
		yaw  = atan2(2.0*(q0*q3+q1*q2) , 1-2.0*(q2*q2+q3*q3));
	}
	
	void ToGet_Gravityweight(float &x,float &y,float &z){
		x = 2*(q1*q3 - q0*q2);
		y = 2*(q0*q1 + q2*q3);
		z = q0*q0 -q1*q1 -q2*q2 +q3*q3;
	}
	
	Vector3f operator*(const Vector3f &v) const{
		float x = v.x * (1-2*(q2*q2+q3*q3)) + v.y * (2*(q1*q2-q0*q3))   + v.z * (2*(q1*q3+q0*q2));
		float y = v.x * (2*(q1*q2+q0*q3))   + v.y * (1-2*(q1*q1+q3*q3)) + v.z * (2*(q2*q3-q0*q1));
		float z = v.x * (2*(q1*q3-q0*q2))   + v.y * (2*(q2*q3+q0*q1))   + v.z * (1-2*(q1*q1+q2*q2));
		return Vector3f(x, y, z);
	}

};



#endif
