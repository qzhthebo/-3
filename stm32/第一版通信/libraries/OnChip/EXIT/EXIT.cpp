#include "EXIT.h"


const EXTITrigger_TypeDef EXITMode::Rising	 = EXTI_Trigger_Rising;
const EXTITrigger_TypeDef EXITMode::Falling  = EXTI_Trigger_Falling;
const EXTITrigger_TypeDef EXITMode::Both		 = EXTI_Trigger_Rising_Falling;

EXIT* EXIT::pEXIT[16] = {0};  

EXIT::EXIT(GPIO_TypeDef *_PORT,u8 _Pin,EXTITrigger_TypeDef _ITMode,
	 GPIOMode_TypeDef _mode, u8 _DefaultLevel,u8 _IT_Group,u8 _prePriority
			,u8 _subPriority):_IT_Pin(_PORT,_Pin,_mode,GPIOSpeed::HIGH,true)
{
	_IT_Pin = _DefaultLevel;
	_IT_Line = u32(1<<_Pin);
	
	u8 portSource;
	if     (_PORT==GPIOA)	portSource = 0; 
	else if(_PORT==GPIOB) 	portSource = 1; 
	else if(_PORT==GPIOC)	portSource = 2; 
	else if(_PORT==GPIOD)	portSource = 3;	
	else if(_PORT==GPIOE)	portSource = 4;	
	else if(_PORT==GPIOF)	portSource = 5;	
	else if(_PORT==GPIOG)	portSource = 6;	
	GPIO_EXTILineConfig(portSource,_Pin);
	
	u8 tmpIRQn[16] = {EXTI0_IRQn,			EXTI1_IRQn,			EXTI2_IRQn,			EXTI3_IRQn,      
	                  EXTI4_IRQn,			EXTI9_5_IRQn,		EXTI9_5_IRQn,		EXTI9_5_IRQn,    
	                  EXTI9_5_IRQn,		EXTI9_5_IRQn,		EXTI15_10_IRQn,	EXTI15_10_IRQn, 
	                  EXTI15_10_IRQn,	EXTI15_10_IRQn,	EXTI15_10_IRQn,	EXTI15_10_IRQn}; 
	
	u32 PriorityGroup[5]={NVIC_PriorityGroup_0,NVIC_PriorityGroup_1,NVIC_PriorityGroup_2,NVIC_PriorityGroup_3,NVIC_PriorityGroup_4};
	NVIC_PriorityGroupConfig(PriorityGroup[_IT_Group]);
	NVIC_InitTypeDef NVIC_InitStructure;                               	
	NVIC_InitStructure.NVIC_IRQChannel = tmpIRQn[_Pin];	              
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _prePriority;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _subPriority;      
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
	NVIC_Init(&NVIC_InitStructure);		
	
	EXTI_InitTypeDef EXTI_InitStructure;							
	EXTI_ClearITPendingBit(_IT_Line);							  
	EXTI_InitStructure.EXTI_Line = _IT_Line;					 	 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = _ITMode;						
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	        
	EXTI_Init(&EXTI_InitStructure);                    
}

void EXIT::IRQ(){
	EXTI_ClearITPendingBit(_IT_Line);
	
	if(_IT_Pin.Get())
		RisingIRQ();
	else FallingIRQ();
}

