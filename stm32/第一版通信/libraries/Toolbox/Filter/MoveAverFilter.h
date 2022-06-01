#ifndef _MoveAverFilter_
#define _MoveAverFilter_


#define MOV_AVERAGE_MAX_COUNT 8
#include "stm32f10x.h"

template<typename T>
class MoveAverageFilter{
protected:
	uint16_t number;
	uint16_t index;
	T input;
	T average[MOV_AVERAGE_MAX_COUNT];
public:
	MoveAverageFilter();
	T Get(T _input);
	T GetAntiPulse(T _input);
};

template<typename T>
MoveAverageFilter<T>::MoveAverageFilter(){
	number = 0;
	index = 0;
}

template<typename T>
T MoveAverageFilter<T>::Get(T _input){
	int64_t sum = 0;
	average[index] = _input;
	index++;
	if(index==MOV_AVERAGE_MAX_COUNT)
		index = 0;
	for(int i=0;i<MOV_AVERAGE_MAX_COUNT;i++){
		sum += average[i];
	}
	return (sum/MOV_AVERAGE_MAX_COUNT);
}

template<typename T>
T MoveAverageFilter<T>::GetAntiPulse(T _input){
	int64_t sum = 0;
	T max=0;
	T min=0;
	if(number==0){
		min = _input;
		max = _input;
	}
	average[index] = _input;
	index++;
	if(index==MOV_AVERAGE_MAX_COUNT)
		index = 0;
	for(int i=0;i<MOV_AVERAGE_MAX_COUNT;i++){
		if(average[i]>max)
			max =average[i];
		else if(average[i]<min)
			min =average[i];
		sum += average[i];
	}
	return (sum-max-min)/(MOV_AVERAGE_MAX_COUNT-2);
}

#endif

