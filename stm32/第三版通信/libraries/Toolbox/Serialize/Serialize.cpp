#include "Serialize.h"

union Convert DataConverter;


void Serialize::FromDouble(double data,u8 ByByte[],bool reverse){
	DataConverter.Bydouble = data;
	for(u8 i=0;i<8;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[7-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}
}


void Serialize::FromFloat(float data,u8 ByByte[],bool reverse){
	DataConverter.Byfloat[0] = data;
	for(u8 i=0;i<4;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[3-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}	
}

void Serialize::FromSigned64(int64_t data,u8 ByByte[],bool reverse){
	DataConverter.BySigned64 = data;
	for(u8 i=0;i<8;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[7-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}		
}


void Serialize::FromSigned32(int32_t data,u8 ByByte[],bool reverse){
	DataConverter.BySigned32[0] = data;
	for(u8 i=0;i<4;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[3-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}		
}

void Serialize::FromSigned16(int16_t data,u8 ByByte[],bool reverse){
	DataConverter.BySigned16[0] = data;
	for(u8 i=0;i<2;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[1-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}			
}

void Serialize::FromUnSigned32(u32 data,u8 ByByte[],bool reverse){
	DataConverter.ByUnSigned32[0] = data;
	for(u8 i=0;i<4;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[3-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}		
}

void Serialize::FromUnSigned16(u16 data,u8 ByByte[],bool reverse){
	DataConverter.ByUnSigned16[0] = data;
	for(u8 i=0;i<2;i++){
		if(reverse)
			ByByte[i] = DataConverter.Bybyte[1-i];
		else
			ByByte[i] = DataConverter.Bybyte[i];
	}		
}

double Serialize::ToDouble(u8 ByByte[],bool reverse){
	for(u8 i=0;i<8;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[7-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.Bydouble;
}

float Serialize::ToFloat(u8 ByByte[],bool reverse){
	for(u8 i=0;i<4;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[3-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.Byfloat[0];	
}

int64_t Serialize::ToSigned64(u8 ByByte[],bool reverse){
	for(u8 i=0;i<8;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[7-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.BySigned64;	
}

int32_t Serialize::ToSigned32(u8 ByByte[],bool reverse){
	for(u8 i=0;i<4;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[3-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.BySigned32[0];		
}

int16_t Serialize::ToSigned16(u8 ByByte[],bool reverse){
	for(u8 i=0;i<2;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[1-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.BySigned16[0];		
}

uint32_t Serialize::ToUnSigned32(u8 ByByte[],bool reverse){
	for(u8 i=0;i<4;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[3-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.ByUnSigned32[0];	
}

uint16_t Serialize::ToUnSigned16(u8 ByByte[],bool reverse){
	for(u8 i=0;i<2;i++){
		if(reverse)
			DataConverter.Bybyte[i] = ByByte[1-i];
		else
			DataConverter.Bybyte[i] = ByByte[i];
	}
	return DataConverter.ByUnSigned16[0];	
}
