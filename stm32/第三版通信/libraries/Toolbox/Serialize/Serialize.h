#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include "stm32f10x.h"

union Convert{
	double Bydouble;
	float Byfloat[2];
	int64_t BySigned64;
	int32_t BySigned32[2];
	u32 ByUnSigned32[2];
	int16_t BySigned16[4];
	u16 ByUnSigned16[4];
	u8 Bybyte[8];
};

class Serialize{
private:
	Serialize(){}
public:
	// data -> data bytes
	static void FromDouble(double data,u8 ByByte[],bool reverse=false);
	static void FromFloat(float data,u8 ByByte[],bool reverse=false);
	static void FromSigned64(int64_t data,u8 ByByte[],bool reverse=false);
	static void FromSigned32(int32_t data,u8 ByByte[],bool reverse=false);
	static void FromSigned16(int16_t data,u8 ByByte[],bool reverse=false);
	static void FromUnSigned32(u32 data,u8 ByByte[],bool reverse=false);
	static void FromUnSigned16(u16 data,u8 ByByte[],bool reverse=false);

	// data bytes -> data
	static double ToDouble(u8 ByByte[],bool reverse=false);
	static float ToFloat(u8 ByByte[],bool reverse=false);
	static int64_t ToSigned64(u8 ByByte[],bool reverse=false);
	static int32_t ToSigned32(u8 ByByte[],bool reverse=false);
	static int16_t ToSigned16(u8 ByByte[],bool reverse=false);
	static u32 ToUnSigned32(u8 ByByte[],bool reverse=false);
	static u16 ToUnSigned16(u8 ByByte[],bool reverse=false);
};



#endif
