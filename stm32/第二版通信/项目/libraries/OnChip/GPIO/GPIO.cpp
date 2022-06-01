
#include "GPIO.h"

//Initialize Parameter
const GPIOSpeed_TypeDef GPIOSpeed::LOW  = GPIO_Speed_2MHz;  //Low speed with 2MHz
const GPIOSpeed_TypeDef GPIOSpeed::MID  = GPIO_Speed_10MHz; //Middle speed with 10MHz
const GPIOSpeed_TypeDef GPIOSpeed::HIGH = GPIO_Speed_50MHz; //High speed with 50MHz

const GPIOMode_TypeDef GPIOMode::IN_Analog   = GPIO_Mode_AIN;      
const GPIOMode_TypeDef GPIOMode::IN_FLOATING = GPIO_Mode_IN_FLOATING; 
const GPIOMode_TypeDef GPIOMode::IN_PD       = GPIO_Mode_IPD;        
const GPIOMode_TypeDef GPIOMode::IN_PU       = GPIO_Mode_IPU;        
const GPIOMode_TypeDef GPIOMode::OUT_OD      = GPIO_Mode_Out_OD;   
const GPIOMode_TypeDef GPIOMode::OUT_PP      = GPIO_Mode_Out_PP;     
const GPIOMode_TypeDef GPIOMode::OUT_OD_AF   = GPIO_Mode_AF_OD;     
const GPIOMode_TypeDef GPIOMode::OUT_PP_AF   = GPIO_Mode_AF_PP;    

GPIO::GPIO(GPIO_TypeDef* port, u16 pinNum, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, bool enableAFIO)
{
	_ioPort = port;            
	_ioPin  = u16(1<<pinNum);   
	
	u32 tmpGPIORCC;  

	if     (_ioPort==GPIOA)	tmpGPIORCC = RCC_APB2Periph_GPIOA; 
	else if(_ioPort==GPIOB)	tmpGPIORCC = RCC_APB2Periph_GPIOB; 
	else if(_ioPort==GPIOC)	tmpGPIORCC = RCC_APB2Periph_GPIOC; 
	else if(_ioPort==GPIOD)	tmpGPIORCC = RCC_APB2Periph_GPIOD; 
	else if(_ioPort==GPIOE)	tmpGPIORCC = RCC_APB2Periph_GPIOE; 
	else if(_ioPort==GPIOF)	tmpGPIORCC = RCC_APB2Periph_GPIOF; 
	else if(_ioPort==GPIOG)	tmpGPIORCC = RCC_APB2Periph_GPIOG; 

	if(enableAFIO)  
		tmpGPIORCC |= RCC_APB2Periph_AFIO;   
	
	RCC_APB2PeriphClockCmd(tmpGPIORCC, ENABLE);               
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = speed;      
	GPIO_InitStructure.GPIO_Mode  = mode;       
	GPIO_InitStructure.GPIO_Pin   = _ioPin;    
	GPIO_Init(_ioPort, &GPIO_InitStructure);   
}

GPIO& GPIO::operator=(bool level)
{
	if(level)
		GPIO_SetBits(_ioPort,_ioPin);  
	else
		GPIO_ResetBits(_ioPort,_ioPin); 
	return (*this);
}

void GPIO::Set(bool level)
{
	if(level)
		GPIO_SetBits(_ioPort,_ioPin);   
	else
		GPIO_ResetBits(_ioPort,_ioPin); 
}

u8 GPIO::Get()
{
	return GPIO_ReadInputDataBit(_ioPort,_ioPin);
}
