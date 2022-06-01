#ifndef _LPF_H_
#define _LPF_H_

template<typename T>
class LPF{
protected:
	float factor;
	T old_data;
	T new_data;
public:
	LPF();
	LPF(float _factor);
	void setFactor(float _factor);
	T Get(T _input);
};
template<typename T>
LPF<T>::LPF(){
	old_data = 0;
	factor = 0.3;
}

template<typename T>
LPF<T>::LPF(float _factor){
	old_data = 0;
	factor = _factor;
}

template<typename T>
void LPF<T>::setFactor(float _factor){
	factor = _factor;
}

template<typename T>
T LPF<T>::Get(T _input){
	new_data = _input;
	return old_data * (1-factor) + new_data * factor;
}



#endif
