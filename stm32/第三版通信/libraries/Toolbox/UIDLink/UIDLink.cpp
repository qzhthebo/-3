#include "UIDLink.h"



UIDLink::UIDLink(UART &com,u8 FunctionList[][3],u8 FunctionNumber,u8 TxHead,u8 TxEnd,u8 RxHead,u8 RxEnd)
:_COM(com),_Rx_Buffer(com._RX_Buffer){
	//FunctionList -> Function Frame List
	//FunctionNumber -> ALL Frame numbers
	_FunctionList = &FunctionList[0][0];
	_Tx_Frame._Head = TxHead;
	_Tx_Frame._End = TxEnd;
	_FunctionNumber = FunctionNumber;
	_Rx_Frame._Head = _Tx_Frame._Head = TxHead;
	_Rx_Frame._End = _Tx_Frame._End = TxEnd;

}



// ======================================  Fill TxFrame with data
void UIDLink::FillDouble(u8 index,double value,bool reverse){
	Serialize::FromDouble(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillFloat(u8 index,float value,bool reverse){
	Serialize::FromFloat(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillSigned64(u8 index,int64_t value,bool reverse){
	Serialize::FromSigned64(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillSigned32(u8 index,int32_t value,bool reverse){
	Serialize::FromSigned32(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillSigned16(u8 index,int16_t value,bool reverse){
	Serialize::FromSigned16(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillUnSigned32(u8 index,uint32_t value,bool reverse){
	Serialize::FromUnSigned32(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillUnSigned16(u8 index,uint16_t value,bool reverse){
	Serialize::FromUnSigned16(value,_Tx_Frame._Data+index,reverse);
}
void UIDLink::FillUnSigned8(u8 index,uint8_t value){
	_Tx_Frame._Data[index] = value;
}



// ====================================== Get data from Rx Frame
double UIDLink::GetDouble(u8 index,bool reverse){
	return Serialize::ToDouble(_Rx_Frame._Data+index,reverse);
}
float UIDLink::GetFloat(u8 index,bool reverse){
	return Serialize::ToFloat(_Rx_Frame._Data+index,reverse);	
}
int64_t UIDLink::GetSigned64(u8 index,bool reverse){
	return Serialize::ToSigned64(_Rx_Frame._Data+index,reverse);	
}
int32_t UIDLink::GetSigned32(u8 index,bool reverse){
	return Serialize::ToSigned32(_Rx_Frame._Data+index,reverse);	
}
int16_t UIDLink::GetSigned16(u8 index,bool reverse){
	return Serialize::ToSigned16(_Rx_Frame._Data+index,reverse);	
}
uint32_t UIDLink::GetUnSigned32(u8 index,bool reverse){
	return Serialize::ToUnSigned32(_Rx_Frame._Data+index,reverse);	
}
uint16_t UIDLink::GetUnSigned16(u8 index,bool reverse){
	return Serialize::ToUnSigned16(_Rx_Frame._Data+index,reverse);	
}
uint8_t UIDLink::GetUnSigned8(u8 index){
	return _Rx_Frame._Data[index];
}


int8_t UIDLink::Pack_Check(){
		//Check Item
		//Header FunctionCode Length CheckSum
	
		//u8 -> u16??
	
	
		//Have RxData And Have FrameHead And FIFO Have Not 
		while(_Rx_Buffer._size_>0 && (_Rx_Buffer._buffer[_Rx_Buffer._index_out]!=_Rx_Frame._Head)){
			if(++_Rx_Buffer._index_out >= _Rx_Buffer._max_size_)
					_Rx_Buffer._index_out = 0;
				_Rx_Buffer._size_--;
		}
		
		//============ ERROR CODE -1 -> Not Enough Data Or Not Fine Frame Head
		if(_Rx_Buffer._size_<5)
			return -1;
		
		//Function Code Index
		uint16_t index = _Rx_Buffer._index_out+1;
		if(index >= _Rx_Buffer._max_size_) index -=_Rx_Buffer._max_size_;
		
		uint8_t FuncCode = _Rx_Buffer._buffer[index];
	
		//ConFirm Function Code
		uint16_t FuncIndex;
		for(FuncIndex=0;FuncIndex<_FunctionNumber;FuncIndex++){
			if(FuncCode == *(_FunctionList+3*FuncIndex))
				break;
		}
		//Not Found FuncCode in FuncList
		if(FuncIndex>=_FunctionNumber){
			//OutIndex Move Forward
			_Rx_Buffer.GetData(FuncCode);
			return -2;
		}
		//============ ERROR CODE -2 -> Function Code ERROR

		index = _Rx_Buffer._index_out+2;
		//dataLength Index
		if(index >= _Rx_Buffer._max_size_) index -= _Rx_Buffer._max_size_;
		//Get DataLength Number
		uint16_t dataLength = _Rx_Buffer._buffer[index];
		//Confirm DataLength
		if(dataLength!=*(_FunctionList+3*FuncIndex+2)){
			//OutIndex Move Forward
			_Rx_Buffer.GetData(FuncCode); 
			return -3;
		}
		//============ ERROR CODE -3 -> Data Length ERROR
		
		if(_Rx_Buffer._size_<dataLength+5){
			return -4;
		}
		//============ ERROR CODE -4 -> Correct Frame,No Enough Data
		
		u8 CheckSum=0;
		//Calculate CheckSum
		for(u8 i=0;i<dataLength+3;i++){
			index = _Rx_Buffer._index_out+i;
			
			if(index>=_Rx_Buffer._max_size_)
				index -= _Rx_Buffer._max_size_;
			
			CheckSum += _Rx_Buffer._buffer[index];
		}
		uint16_t endIndex = index+2;
		if(endIndex >= _Rx_Buffer._max_size_) endIndex -= _Rx_Buffer._max_size_;
		CheckSum += _Rx_Buffer._buffer[endIndex];
		
		if(++index>=_Rx_Buffer._max_size_) index -=_Rx_Buffer._max_size_;
		if(CheckSum!=_Rx_Buffer._buffer[index]){
			_Rx_Buffer.GetData(FuncCode);
			return -5;
		}
		//============ ERROR CODE -5 -> CheckSum ERROR
		
		if(++index>=_Rx_Buffer._max_size_) index -=_Rx_Buffer._max_size_;
		if(_Rx_Buffer._buffer[index]!=_Rx_Frame._End){
			_Rx_Buffer.GetData(FuncCode);
			return -6;
		}
		//============ ERROR CODE -6 -> Frame End ERROR
		
		
		//Success to Check Frame, Fill this
		_Rx_Buffer.GetData(_Rx_Frame._Head);
		_Rx_Buffer.GetData(_Rx_Frame._FunctionCode);
		_Rx_Buffer.GetData(_Rx_Frame._DataLength);
		_Rx_Buffer.GetDatas(_Rx_Frame._Data,dataLength);
		_Rx_Buffer.GetData(_Rx_Frame._Sum);
		_Rx_Buffer.GetData(_Rx_Frame._End);
		
		return _Rx_Frame._FunctionCode;
}	







bool UIDLink::Pack_Send(){
	
	//Check Function Code
	u8 index;
	for(index=0;index<_FunctionNumber;index++){
			if(_Tx_Frame._FunctionCode == *(_FunctionList+index*3))
				break;
	}
	//=========================== ERROR -> Not Found Function Code
	if(index>=_FunctionNumber) return false;
	
	//Fill DataLength
	_Tx_Frame._DataLength = *(_FunctionList+index*3+1);
	
	//Calculate Check Sum
	_Tx_Frame._Sum = _Tx_Frame._Head + _Tx_Frame._DataLength + _Tx_Frame._FunctionCode + _Tx_Frame._End;
	for(index=0;index<_Tx_Frame._DataLength;index++){
		_Tx_Frame._Sum += _Tx_Frame._Data[index];
	}
	
	//Send Tx Frame
	_COM.SendByte(_Tx_Frame._Head);
	_COM.SendByte(_Tx_Frame._FunctionCode);
	_COM.SendByte(_Tx_Frame._DataLength);
	_COM.SendBytes(_Tx_Frame._Data,_Tx_Frame._DataLength);
	_COM.SendByte(_Tx_Frame._Sum);
	_COM.SendByte(_Tx_Frame._End);
	return true;
}

