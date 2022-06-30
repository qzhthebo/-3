#ifndef _FIFO_BUFFER_H
#define _FIFO_BUFFER_H

#include "stm32f10x.h"

template<typename T, u16 MAXSIZE>
class FIFOBuffer{
public:
	T _buffer[MAXSIZE];
	u16 _max_size_;  //buffer max size
	u16 _size_;		 //buffer current size
	u16 _index_out;  //data index of get out
	u16 _index_in;   //data index of putting in
public:
	FIFOBuffer();
	bool PutData(const T &data);  		//put one data
	bool GetData(T &data);		  		//get one data
	bool PutDatas(T *datas,u16 number); //put datas
	bool GetDatas(T *datas,u16 number); //get datas
	u16 size();							//get current size of buffer
	void flush();						//flush buffer
	u16 MaxSize();						//the max byte size of buffer
	u16 RestOfSize();					//the rest of buffer size			
};

//Construct Function
template<typename T,u16 MAXSIZE>
FIFOBuffer<T,MAXSIZE>::FIFOBuffer(){
	_max_size_ = MAXSIZE;
	_size_ = 0;
	_index_out = 0;
	_index_in = 0;
}


template<typename T,u16 MAXSIZE>
bool FIFOBuffer<T,MAXSIZE>::PutData(const T &data){
	if(_size_ >= _max_size_)
		return false;
	_buffer[_index_in++] = data;
	if(_index_in >= _max_size_)
		_index_in = 0;
	_size_++;
	return true;
}

template<typename T,u16 MAXSIZE>
bool FIFOBuffer<T,MAXSIZE>::PutDatas(T *datas,u16 number){
	if((_size_+number) > _max_size_)
		return false;
	for(u16 i=0;i<number;i++){
		_buffer[_index_in++] = datas[i];
		if(_index_in >= _max_size_)
			_index_in = 0;
	}
	_size_ += number;
	return true;
}

template<typename T,u16 MAXSIZE>
bool FIFOBuffer<T,MAXSIZE>::GetData(T &data){
	if(_size_ <=0 )
		return false;
	data = _buffer[_index_out++];
	if(_index_out >= _max_size_)
		_index_out = 0;

	_size_--;
	return true;
}

template<typename T,u16 MAXSIZE>
bool FIFOBuffer<T,MAXSIZE>::GetDatas(T *datas,u16 number){
	if(_size_ < number) 
		return false;
	for(u16 i=0;i<number;i++){
		datas[i] = _buffer[_index_out++];
		if(_index_out >= _max_size_)
			_index_out = 0;
	}
	_size_ -= number;
	return true;
}

template<typename T,u16 MAXSIZE>
u16 FIFOBuffer<T,MAXSIZE>::size(){
	return _size_;
}

template<typename T,u16 MAXSIZE>
u16 FIFOBuffer<T,MAXSIZE>::MaxSize(){
	return _max_size_;
}

template<typename T,u16 MAXSIZE>
u16 FIFOBuffer<T,MAXSIZE>::RestOfSize(){
	return (_max_size_ - _size_);
}

template<typename T,u16 MAXSIZE>
void FIFOBuffer<T,MAXSIZE>::flush(){
	_size_ = 0;
	_index_out = 0;
	_index_in = 0;
}


#endif
