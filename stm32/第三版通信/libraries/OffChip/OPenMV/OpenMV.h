#ifndef _OPENMV_H_
#define _OPENMV_H_

#include "stm32f10x.h"
#include "UART.h"
#include "UIDLink.h"
extern UIDLink _UIDLink;


class OpenMV{
	public:
	float distance;
	u16 cx;
	u16 cy;
	u16 lx1;
	u16 lx2;	
	int update();
};

#endif 
