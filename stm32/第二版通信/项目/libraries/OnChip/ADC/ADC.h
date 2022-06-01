#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"


class ADC{
private:
	static u16 _ADC_Value[18];
	static u8  _ADC_Channel_List[18];
	static u8  _ADC_Channel_Count;
private:
	float _k;
	u8 _Order_Number;
	u8 _ADC_Channel_Number;
private:
	static void InitDMA();
	static void InitADC();
public:
	u8 adc_cnt;
	float Vol;
	int Is_LowVol();
	void Updata(int cnt);
	ADC(u8 ChannelNumber,float k=1);
	static void Start();
	float Value();

};


#endif
