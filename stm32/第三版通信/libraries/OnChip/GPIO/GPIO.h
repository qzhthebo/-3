#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"


struct GPIOSpeed
{ 
public:
	static const GPIOSpeed_TypeDef LOW;    // 2MHz
  static const GPIOSpeed_TypeDef MID;    // 10MHz
  static const GPIOSpeed_TypeDef HIGH;   // 50MHz
private:
	GPIOSpeed() {}
};



struct GPIOMode
{ 
public:
	static const GPIOMode_TypeDef IN_Analog;     
	static const GPIOMode_TypeDef IN_FLOATING; 
	static const GPIOMode_TypeDef IN_PD;        
	static const GPIOMode_TypeDef IN_PU;       
	static const GPIOMode_TypeDef OUT_OD;      
	static const GPIOMode_TypeDef OUT_PP;      
  static const GPIOMode_TypeDef OUT_OD_AF;   
	static const GPIOMode_TypeDef OUT_PP_AF;    
private:
	GPIOMode() {}
};

class GPIO
{
protected:
	GPIO_TypeDef*  _ioPort;   
	u16            _ioPin;    
public:
  GPIO(GPIO_TypeDef* port, u16 pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed=GPIOSpeed::MID, bool enableAFIO=false);	
	
	GPIO& operator=(bool level);
  void Set(bool level);

  u8 Get();
};



#endif


