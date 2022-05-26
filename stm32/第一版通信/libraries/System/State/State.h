#ifndef _STATE_H_
#define _STATE_H_

#include "UART.h"    		
#include "SysTime.h" 		
#include "GPIO.h"
#include "UIDLink.h"
#include "Communication.h"
#include "SBUS.h"
#include "RemoteControl.h"
#include "Control.h"
#include "OpenMV.h"
#include "Safety.h"
#include "ADC.h"
//#include "buzzer.h"

#define INIT 0
#define STANDBY 1
#define RUNNING 2
#define ERROR 3
#define CAL 4

//extern  buzzer b;
extern SBUS _sbus;
extern ADC _adc;
extern RemoteControl _remote;	
extern GPIO PB11;
class State{
	public:
	static int state;
	static void Initialize();
	static void Updata();
};


#endif


