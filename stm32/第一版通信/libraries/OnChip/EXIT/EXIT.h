#ifndef _EXIT_H_
#define _EXIT_H_

#include "stm32f10x.h"
#include "GPIO.h"

struct EXITMode
{
public:
  static const EXTITrigger_TypeDef Rising;   
  static const EXTITrigger_TypeDef Falling;  
  static const EXTITrigger_TypeDef Both;   
 private:
  EXITMode(){}  
};

class EXIT{
protected:
	GPIO _IT_Pin;
	u32 _IT_Line;
public:
	static EXIT* pEXIT[16];
public:
	EXIT(GPIO_TypeDef *_PORT,u8 _Pin,EXTITrigger_TypeDef _ITMode,
		 GPIOMode_TypeDef _mode, u8 _DefaultLevel = 1,
		 u8 _IT_Group=3,u8 _prePriority=5,u8 _subPriority=0);

	void IRQ();
protected:
	virtual void RisingIRQ(){};
	virtual void FallingIRQ(){};
};



#endif
