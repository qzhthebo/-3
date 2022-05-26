#include "SysTime.h"

#include "GPIO.h"

extern GPIO LED2;

//Initialize Param
double SysTime::_interruptTime        = 0;    
double SysTime::_oldTime       			  = 0;     
double SysTime::_currentTime          = 0;     
double SysTime::_deltaT               = 0;    
double SysTime::_lastTimer[][20]      = {0};   
int    SysTime::_countTimer           = 0;     
bool   SysTime::_IsReady              = false; 


//=========================================    Initialize System Time
void SysTime::Initialize()
{
	SysTick->CTRL &= 0xFFFFFFF8; 
	SysTick->LOAD  = 16200000;  
	SysTick->VAL   = 16200000;  
	SysTick->CTRL |= 0x00000003; 
	_IsReady = true;          
}	

//=========================================    Get Current Time
double SysTime::Now()
{
	_currentTime = _interruptTime + 1.8 - SysTick->VAL/9000000.0; 
		
	//check and adjust
	_deltaT = _currentTime - _oldTime;
	if(_deltaT < -1)     
		//Overflow: count-down register reloaded(-1.8s), while interrupt not respons yet
		//current time should add 1.8s
		_currentTime += 1.8; 
	else if(_deltaT>1.79) 
		//Overflow: interrupt responsed, while count-down register not reload yet
		//current time should minus 1.8s
		_currentTime -= 1.8; 
	
	_oldTime = _currentTime;  
	return _currentTime;
}

//=========================================    IRQ Handler
void SysTime::IRQ()
{
	_interruptTime += 1.8;   //each 1.8s count-down overflow 
}

extern "C"
{
	void SysTick_Handler(void)
	{
		SysTime::IRQ();
	}
}



//=========================================    Create Soft Timer 
int SysTime::CreateTimer(double interval){
	if(!_IsReady){
		Initialize();    
	}

	if(_countTimer>19)    
		return -1;       
	
	_lastTimer[0][_countTimer] = interval;  
	_lastTimer[1][_countTimer] = Now();    
	return _countTimer++; 
}


//=========================================    Check Timer 
bool SysTime::CheckTimer(u8 hTimer){
	if(Now()-_lastTimer[1][hTimer] > _lastTimer[0][hTimer])  {
		_lastTimer[1][hTimer] = _currentTime;  
		return true; 
	}
	return false; 
}

void SysTime::delay(double interval){
	double last_time = Now();
	while(Now()-last_time < interval);
}
