#ifndef _DAC_H_
#define _DAC_H_

#include "stm32f10x.h"
#include "GPIO.h"

class CDAC{
private:
	u8 _DAC_Channel_Number;
public:
	CDAC(u8 ChannelNumber);
	void SetVoltage(float voltage);
}



#endif
