#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#include "stm32f10x.h"

class SysTime
{
private:
	static double _oldTime;        
  static double _currentTime; 
	static double _interruptTime;
  static double _deltaT;
  static double _lastTimer[2][20];
  static int    _countTimer; 
  static bool   _IsReady;
private:
	SysTime() {}  
public:		
	static void Initialize(); 
	static double Now(); 
	static void IRQ(); 
	static int  CreateTimer(double interval); 
  static bool CheckTimer(u8 hTimer);
	static void delay(double interval);
};

#endif
