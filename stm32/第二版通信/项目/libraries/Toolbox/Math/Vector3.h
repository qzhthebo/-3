#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <math.h>

template<typename T>
class Vector3{
public:
	T x,y,z;
public:
	//========================== Constructor =========================//
	Vector3(){
		x=0;y=0;z=0;
	}
	Vector3(T x0,T y0, T z0):x(x0),y(y0),z(z0){}
	//========================== Constructor =========================//
	//overload "()"
	void operator()(T x0,T y0, T z0){
		x=x0;y=y0,z=z0;
	}
	//Behind overload "=="
	bool operator==(const Vector3<T> &v){
		return (x==v.x && y==v.y && z==v.z);
	}
	//Behind overload "!="
	bool operator!=(const Vector3<T> &v){
		return (x!=v.x && y!=v.y && z!=v.z);
	}
	//Front overload "-"
	Vector3<T> operator-(void) const{
		return Vector3<T>(-x,-y,-z);
	}
	//Behind overload "+"
	Vector3<T> operator+(const Vector3<T> &v) const{
		return Vector3<T>(x+v.x, y+v.y, z+v.z);
	}
	//Behind overload "-"
	Vector3<T> operator-(const Vector3<T> &v) const{
		return Vector3<T>(x-v.x, y-v.y, z-v.z);
	}
	//Behind overload "*"
	Vector3<T> operator*(const T n) const{
		return Vector3<T>(x*n, y*n, z*n);
	}
	//Behind overload "/"
	Vector3<T> operator/(const T n) const{
		return Vector3<T>(x/n, y/n, z/n);
	}	
	//Behind overload "+="
	Vector3<T> &operator+=(const Vector3<T> &v){
		 x+=v.x; y+=v.y; z+=v.z; 
		 return *this; 
	}
	//Behind overload "-="
	Vector3<T> &operator-=(const Vector3<T> &v){
		 x-=v.x; y-=v.y; z-=v.z; 
		 return *this; 
	}
	//Behind overload "*="
	Vector3<T> &operator*=(const T n){
		x*=n; y*=n; z*=n;		
		return *this;
	}
	//Behind overload "/="
	Vector3<T> &operator/=(const T n){
		x/=n; y/=n; z/=n;		
		return *this;
	}	
	//Vector dot multiply overload
	T operator*(const Vector3<T> &v) const{
		return x*v.x + y*v.y + z*v.z;
	}
	//Vector cross multiply overload
	Vector3<T> operator%(const Vector3<T> &v) const{
		return Vector3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
	}

	//Get Vector3 norm's square
	T lengthSquared() const{
		return (T)(*this * *this);
	}

	//Get Vector3 norm
	float length(void) const{
		return (T)sqrt(*this * *this);
	}

	//normalizes this vector
	void Normalize(){
		*this/=length();
	}

	void Zero(){
		x = y = z = 0.0;
	}

	//Return Normalized vector
	Vector3<T>  Normalized() const {
	   return *this/length();   
	}

	//Check Vector is number or not
	bool isNan(void){
		return isnan(x) || isnan(y) || isnan(z);
	}

	//Check Vector number is too big or not
	bool isInf(void){
		return isinf(x) || isinf(y) || isinf(z);
	}	

};

typedef Vector3<float> Vector3f;




#endif
