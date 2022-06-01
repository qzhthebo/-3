#ifndef _UID_LINK_H_
#define _UID_LINK_H_

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "UART.h"
#include "Serialize.h"

//================================================== Frame Structure
#pragma pack(1)
struct UIDFrame
{
	u8 _Head;
	u8 _FunctionCode;
	u8 _DataLength;
	u8 _Data[300];
	u8 _Sum;
	u8 _End;
};
#pragma pack()


class UIDLink
{
private:
	UART &_COM;
	u8 *_FunctionList;
	u8 _FunctionNumber;
	FIFOBuffer<u8,500> &_Rx_Buffer;
public:
	UIDFrame _Tx_Frame;
	UIDFrame _Rx_Frame;
public:
	UIDLink(UART &com,u8 FunctionList[][3],u8 FunctionNumber,u8 TxHead,u8 TxEnd,u8 RxHead=0x00,u8 RxEnd=0x00);

public:
	// Fill TxFrame with data
	void FillDouble(u8 index,double value,bool reverse=false);
	void FillFloat(u8 index,float value,bool reverse=false);
	void FillSigned64(u8 index,int64_t value,bool reverse=false);
	void FillSigned32(u8 index,int32_t value,bool reverse=false);
	void FillSigned16(u8 index,int16_t value,bool reverse=false);
	void FillUnSigned32(u8 index,uint32_t value,bool reverse=false);
	void FillUnSigned16(u8 index,uint16_t value,bool reverse=false);
	void FillUnSigned8(u8 index,uint8_t value);

	//Get data from Rx Frame
	double GetDouble(u8 index,bool reverse=false);
	float GetFloat(u8 index,bool reverse=false);
	int64_t GetSigned64(u8 index,bool reverse=false);
	int32_t GetSigned32(u8 index,bool reverse=false);
	int16_t GetSigned16(u8 index,bool reverse=false);
	uint32_t GetUnSigned32(u8 index,bool reverse=false);
	uint16_t GetUnSigned16(u8 index,bool reverse=false);
	uint8_t GetUnSigned8(u8 index);

	//share memory
	template<typename T>
	void PtrtoTxData(T &pointer){
		pointer = (T)_Tx_Frame._Data;
	}
	template<typename T>
	void PtrtoRxData(T &pointer){
		pointer = (T)_Rx_Frame._Data;
	}	

	//Copy Operation
	template<typename T>
	void CopyFromRxData(T &pointer){
		pointer = *( (T*)(_Rx_Frame._Data));
	}
	template<typename T>
	void CopyFromTxData(T &pointer){
		*( (T*)(_Tx_Frame._Data)) = pointer;
	}	

	int8_t Pack_Check();
	bool Pack_Send();
};



#endif
