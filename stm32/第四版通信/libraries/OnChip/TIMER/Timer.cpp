#include "Timer.h"


Timer* Timer::pTIM[8]={0};



//	=======================================================   TIMER MODE
Timer::Timer(u8 TimNumber,float ts,void (*FuncIRQ)(),u8 PrePri,u8 SubPri,u8 ItGroup){
	//Check TimeNumber
	if(TimNumber>8) return;
	pTIM[TimNumber-1]=this;
	_TIM_MODE = TIM_MODE_TIMER;
	_UpdateIRQ = FuncIRQ;		//IRQ Function -> given outside
	InitTimeBase(TimNumber,ts,TIM_CounterMode_Down);
	//IRQChannel:  TIM1 				TIM2 				TIM3 			TIM4		 TIM5 	TIM6 			TIM7 			TIM8
	IRQn irq[8]={TIM1_UP_IRQn, TIM2_IRQn,TIM3_IRQn,IRQn(30),IRQn(50),IRQn(54),IRQn(55),IRQn(44)};
	InitNVIC(irq[TimNumber-1],PrePri,SubPri,ItGroup);
	TIM_ClearFlag(_TIMx,TIM_FLAG_Update);
	TIM_ITConfig(_TIMx,TIM_IT_Update,ENABLE);
}


//	=======================================================   COUNTER MODE

Timer::Timer(u8 TimNumber,u16 ExtiTriggerPsc,bool remap){
	if(TimNumber ==5 ||	TimNumber ==6 || TimNumber == 7 || TimNumber>8) return;	
	pTIM[TimNumber-1]=this;
	_TIM_MODE=TIM_MODE_COUNTER;
	
	//Initialize GPIO
	if(!remap){
		GPIO_TypeDef* _PORT[8] = {GPIOA,	GPIOA,	GPIOD,	GPIOE, 0, 0, 0, GPIOA};
		u16						_PIN[8]	 = { 12  ,    0  ,    2  ,    0  , 0, 0, 0,   0};
		GPIO  EnCounter(_PORT[TimNumber-1],_PIN[TimNumber-1],GPIO_Mode_IN_FLOATING);
	}else if(TimNumber ==1 || TimNumber ==2){
		GPIO_TypeDef* _PORT[2] = {GPIOE,GPIOA};
		u16           _PIN[2]	 = {   7 ,  15 };
		GPIO  EnCounter(_PORT[TimNumber-1],_PIN[TimNumber-1],GPIO_Mode_IN_FLOATING);
		u32     			RemapTIM[2]={GPIO_FullRemap_TIM1,GPIO_FullRemap_TIM2};
		GPIO_PinRemapConfig(RemapTIM[TimNumber-1],ENABLE);
		if(TimNumber==2)
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	}else	return;
	
	//Initialize Time Bass
	InitTimeBase(TimNumber, 1 ,TIM_CounterMode_Up,true);
	//EnCounter Config
	TIM_ETRClockMode2Config(_TIMx,ExtiTriggerPsc,TIM_ExtTRGPolarity_NonInverted,5);
	TIM_SetCounter(_TIMx,0);
	
	//Interrupt Config
	//IRQChannel:  TIM1 				TIM2 				TIM3 			TIM4		 TIM5 	TIM6 			TIM7 			TIM8
	IRQn irq[8]={TIM1_UP_IRQn, TIM2_IRQn,TIM3_IRQn,IRQn(30),IRQn(50),IRQn(54),IRQn(55),IRQn(44)};
	InitNVIC(irq[TimNumber-1],3,0,3);
	//Interrupt Source Config
	TIM_ClearITPendingBit(_TIMx,TIM_IT_Update);
	TIM_ITConfig(_TIMx,TIM_IT_Update,ENABLE);
}

//	=======================================================   PWM MODE
Timer::Timer(u8 TimNumber,float FeqHz,bool isPWM,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4,bool remap,u8 PrePri,u8 SubPri,u8 ItGroup){
	if(TimNumber>8 || TimNumber==6 ||TimNumber==7)  return;
	pTIM[TimNumber-1]=this;
	if(isPWM){
		_TIM_MODE=TIM_MODE_PWM;
		InitGPIO(TimNumber,GPIO_Mode_AF_PP,remap,EnCH1,EnCH2,EnCH3,EnCH4);
		InitTimeBase(TimNumber,1.0f/FeqHz,TIM_CounterMode_Down);
		InitOC(true,EnCH1,EnCH2,EnCH3,EnCH4);
	}else{
		_TIM_MODE=TIM_MODE_IPC;
		InitGPIO(TimNumber,GPIO_Mode_IN_FLOATING,remap,EnCH1,EnCH2,EnCH3,EnCH4);
		InitTimeBase(TimNumber,1.0f/FeqHz,TIM_CounterMode_Up);	
		//IRQ Channel:     TIM1       TIM2   		TIM3     	TIM4      TIM5      TIM6      TIM7     TIM8
		IRQn irq[8] = {TIM1_CC_IRQn,TIM2_IRQn,TIM3_IRQn,IRQn(30), IRQn(50), IRQn(54), IRQn(55), IRQn(46)};
		InitNVIC(irq[TimNumber-1],PrePri,SubPri,ItGroup);
		InitIC(TIM_ICPolarity_Rising,EnCH1,EnCH2,EnCH3,EnCH4);
		_Resolution=(u16(72000000.0/65535.0*FeqHz))+1/72000.0;
	}
}

//	=======================================================   ENCODER MODE
Timer::Timer(u8 TimNumber,bool remap,u8 PrePri,u8 SubPri,u8 ItGroup){
	if(TimNumber>8 || TimNumber==6 || TimNumber==7) return;
	pTIM[TimNumber-1]=this;
	_TIM_MODE = TIM_MODE_ENCODER;
	InitGPIO(TimNumber,GPIO_Mode_IN_FLOATING,remap,true,true,false,false);
	InitTimeBase(TimNumber,1,TIM_CounterMode_Up,true);
	//IRQChannel:  TIM1 				TIM2 				TIM3 			TIM4		 TIM5 	TIM6 			TIM7 			TIM8
	IRQn irq[8]={TIM1_UP_IRQn, TIM2_IRQn,TIM3_IRQn,IRQn(30),IRQn(50),IRQn(54),IRQn(55),IRQn(44)};
	InitNVIC(irq[TimNumber-1],PrePri,SubPri,ItGroup);
	TIM_EncoderInterfaceConfig(_TIMx,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//Init Default
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter=6;
	TIM_ICInit(_TIMx,&TIM_ICInitStructure);
	TIM_ClearFlag(_TIMx,TIM_FLAG_Update);
	TIM_ITConfig(_TIMx,TIM_IT_Update,ENABLE);
	TIM_SetCounter(_TIMx,0);
	_EncoderZero=0;
}


//	=======================================================   Frequence MODE

Timer::Timer(u8 TimNumber,bool remap,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4){
	if(TimNumber>8 || TimNumber==6 ||TimNumber==7) return;
	pTIM[TimNumber-1]=this;
	
	_TIM_MODE=TIM_MODE_FRQ;
	InitGPIO(TimNumber,GPIO_Mode_AF_PP,remap,EnCH1,EnCH2,EnCH3,EnCH4);
	InitTimeBase(TimNumber,50,TIM_CounterMode_Down);
	InitOC(false,EnCH1,EnCH2,EnCH3,EnCH4);
	TIM_ARRPreloadConfig(_TIMx,DISABLE);
}


//	=======================================================   Init	Timer 
void Timer::InitTimeBase(u8 TimNumber,float ts,u16 CountMode, bool isEncoder){
	// RCC Config
	u32 RCC_[8]={RCC_APB2Periph_TIM1,RCC_APB1Periph_TIM2,RCC_APB1Periph_TIM3,
		            RCC_APB1Periph_TIM4,RCC_APB1Periph_TIM5,RCC_APB1Periph_TIM6,
		            RCC_APB1Periph_TIM7,RCC_APB2Periph_TIM8};
	if(TimNumber==1 || TimNumber==8)
		RCC_APB2PeriphClockCmd(RCC_[TimNumber-1],ENABLE);
	else
		RCC_APB1PeriphClockCmd(RCC_[TimNumber-1],ENABLE);
	
	//Calculate Period
	u16 prescaler;
	double count=ts*72000000.0;
	prescaler = u16(count/65536.0);
	_Period = u16(count/(prescaler+1))-1;
	if(isEncoder){
		_Period=60000;
		prescaler=0;
	}
	//TIM Config
	TIM_TimeBaseInitTypeDef TIM_BashInitStructure;
	TIM_BashInitStructure.TIM_Period = _Period;
	TIM_BashInitStructure.TIM_Prescaler = prescaler;
	TIM_BashInitStructure.TIM_ClockDivision = 0;
	TIM_BashInitStructure.TIM_RepetitionCounter = 0;
	TIM_BashInitStructure.TIM_CounterMode = CountMode;
	//Initialize TIM
	TIM_TypeDef *tmpTIMx[8] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8};
	_TIMx=tmpTIMx[TimNumber-1];
	TIM_TimeBaseInit(_TIMx,&TIM_BashInitStructure);
	TIM_Cmd(_TIMx,ENABLE);
}

//	=======================================================   Init	NVIC 
void Timer::InitNVIC(IRQn irqChannel,u8 PrePri,u8 SubPri,u8 ItGroup){
	u32 priorityGroup[5] = {NVIC_PriorityGroup_0,NVIC_PriorityGroup_1,NVIC_PriorityGroup_2,NVIC_PriorityGroup_3,NVIC_PriorityGroup_4};
	NVIC_PriorityGroupConfig(priorityGroup[ItGroup]);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = irqChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPri;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//	=======================================================   Init	GPIO 
void Timer::InitGPIO(u8 TimNumber,GPIOMode_TypeDef mode,bool remap,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4){
	
	if(remap==true && TimNumber<=4){
		//full remap
		//GPIO full remap                TIM1   	TIM2  	TIM3  	TIM4
		GPIO_TypeDef* PORT_Channel_1[4]={GPIOE	,	GPIOA,	GPIOC,	GPIOD};
		u16           PIN_Channel_1[4] ={  9  	,  15 ,  		6  , 	 12 }; 
		GPIO_TypeDef* PORT_Channel_2[4]={GPIOE	,	GPIOB,	GPIOC,	GPIOD};
		u16           PIN_Channel_2[4] ={  11   ,  3 ,  		7  , 	 13 }; 
		GPIO_TypeDef* PORT_Channel_3[4]={GPIOE	,	GPIOB,	GPIOC,	GPIOD};
		u16           PIN_Channel_3[4] ={  13   ,  10 ,  		8  ,	  14}; 
		GPIO_TypeDef* PORT_Channel_4[4]={GPIOE	,	GPIOB,	GPIOC,	GPIOD};
		u16           PIN_Channel_4[4] ={  14   ,  11 ,  		9  , 	  15}; 		
		if(EnCH1) GPIO CH1_(PORT_Channel_1[TimNumber-1],PIN_Channel_1[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		if(EnCH2) GPIO CH2_(PORT_Channel_2[TimNumber-1],PIN_Channel_2[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		if(EnCH3) GPIO CH3_(PORT_Channel_3[TimNumber-1],PIN_Channel_3[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		if(EnCH4) GPIO CH4_(PORT_Channel_4[TimNumber-1],PIN_Channel_4[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		
		u32 RemapTIM[4] = {GPIO_FullRemap_TIM1, GPIO_FullRemap_TIM2, GPIO_FullRemap_TIM3, GPIO_Remap_TIM4};
		GPIO_PinRemapConfig(RemapTIM[TimNumber-1],ENABLE);
		if(TimNumber==2)
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	}else{
		//GPIO                 						TIM1  		 TIM2  TIM3  	TIM4
		GPIO_TypeDef* PORT_Channel_1[8]={GPIOA	,	GPIOA,	GPIOA,	GPIOB,	GPIOA,	0,	0,	GPIOC};
		u16           PIN_Channel_1[8] ={  8  ,  		0 ,  		6  ,  	6 ,			0,		0,	0,		6	 }; 
		GPIO_TypeDef* PORT_Channel_2[8]={GPIOA	,	GPIOA,	GPIOA,	GPIOB,	GPIOA,	0,	0,	GPIOC};
		u16           PIN_Channel_2[8] ={  9  ,  		1 , 		7  ,  	7 ,			1,		0,	0,		7	 }; 
		GPIO_TypeDef* PORT_Channel_3[8]={GPIOA,		GPIOA,	GPIOB,	GPIOB,	GPIOA,	0,	0,	GPIOC};
		u16           PIN_Channel_3[8] ={  10  ,  	2 ,  		0  ,  	8,			2,		0,	0,		8	 }; 
		GPIO_TypeDef* PORT_Channel_4[8]={GPIOA,		GPIOA,	GPIOB,	GPIOB,	GPIOA,	0,	0,	GPIOC};
		u16           PIN_Channel_4[8] ={  11  ,  	3 , 	  1  ,  	9,			3,		0,	0,		9	 }; 		
		if(EnCH1) GPIO CH1_(PORT_Channel_1[TimNumber-1],PIN_Channel_1[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		if(EnCH2) GPIO CH2_(PORT_Channel_2[TimNumber-1],PIN_Channel_2[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		if(EnCH3) GPIO CH3_(PORT_Channel_3[TimNumber-1],PIN_Channel_3[TimNumber-1],mode,GPIO_Speed_50MHz,true);
		if(EnCH4) GPIO CH4_(PORT_Channel_4[TimNumber-1],PIN_Channel_4[TimNumber-1],mode,GPIO_Speed_50MHz,true);		
	}
}



//	=======================================================   Init	Output Capture 
void Timer::InitOC(bool isPWM,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4){
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	if(isPWM)
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	else
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  //Frequency Mode
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //Enable Output
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	if(EnCH1){
		TIM_OC1Init(_TIMx,&TIM_OCInitStructure);
		TIM_OC1PreloadConfig(_TIMx,TIM_OCPreload_Enable);
	}
	if(EnCH2){
		TIM_OC2Init(_TIMx,&TIM_OCInitStructure);
		TIM_OC2PreloadConfig(_TIMx,TIM_OCPreload_Enable);
	}
	if(EnCH3){
		TIM_OC3Init(_TIMx,&TIM_OCInitStructure);
		TIM_OC3PreloadConfig(_TIMx,TIM_OCPreload_Enable);
	}
	if(EnCH4){
		TIM_OC4Init(_TIMx,&TIM_OCInitStructure);
		TIM_OC4PreloadConfig(_TIMx,TIM_OCPreload_Enable);
	}
	if(_TIMx==TIM1 || _TIMx==TIM8)
		TIM_CtrlPWMOutputs(_TIMx,ENABLE);
}



//	=======================================================   Init	Input Capture 
void Timer::InitIC(u16 IcEdge,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4){
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_ICPolarity = IcEdge;   	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     
	TIM_ICInitStructure.TIM_ICFilter = 0; 
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(_TIMx,&TIM_ICInitStructure);
	TIM_ITConfig(_TIMx,TIM_IT_CC1,EnCH1?ENABLE:DISABLE);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(_TIMx,&TIM_ICInitStructure);
	TIM_ITConfig(_TIMx,TIM_IT_CC2,EnCH2?ENABLE:DISABLE);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(_TIMx,&TIM_ICInitStructure);
	TIM_ITConfig(_TIMx,TIM_IT_CC3,EnCH3?ENABLE:DISABLE);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(_TIMx,&TIM_ICInitStructure);
	TIM_ITConfig(_TIMx,TIM_IT_CC4,EnCH4?ENABLE:DISABLE);
}




void Timer::SetDutyRadio(u8 ChannelNumber,float duty){
	switch(ChannelNumber){
		case 1: _TIMx->CCR1 = _Period*duty/100.f; break;
		case 2: _TIMx->CCR2 = _Period*duty/100.f; break;
		case 3: _TIMx->CCR3 = _Period*duty/100.f; break;
		case 4: _TIMx->CCR4 = _Period*duty/100.f; break;
	}
}
void Timer::SetDutyRadio(float channel1,float channel2,float channel3,float channel4){
		 _TIMx->CCR1 = _Period*channel1/100.f; 
		 _TIMx->CCR2 = _Period*channel2/100.f; 
		 _TIMx->CCR3 = _Period*channel3/100.f; 
		 _TIMx->CCR4 = _Period*channel4/100.f;
}

double Timer::InputCaptureValue(u8 ChannelNumber){
	return _IPC_Count[ChannelNumber-1]*_Resolution;
}


int64_t Timer::Counts(){
	return _EncoderPulse + u32(_TIMx->CNT);
}
void Timer::Encoder_IRQ(){
	_EncoderZero = Counts();
}


void Timer::SetHz(float hz){
	if(hz<1) hz=1;
	else if(hz>300000) hz=300000;
	
	float count = 36000000.0f/hz;
	u16 prescaler = u16(count/65536.0f);
	u16 period = u16(count/(prescaler+1))-1;
	
	_TIMx->PSC = prescaler;
	_TIMx->ARR = period;
}

void Timer::Enable(bool enable){
	if(enable)
		TIM_Cmd(_TIMx,ENABLE);
	else TIM_Cmd(_TIMx,DISABLE);
}




void Timer::InputCaptureIRQ()
{
	static bool isRising[4] = {true, true, true,true};	//Rising or falling interrupt flag, inverse each time
	static u16 cntRising[4] = {0},ccr;									//for saving capture count
	
	//Channel 1
	if(TIM_GetITStatus(_TIMx, TIM_IT_CC1) != RESET)						//Check Capture Interrupt Bit
	{
		ccr = TIM_GetCapture1(_TIMx);					//Get Capture Count
		if(isRising[0]) 		   								//Rising edge interrupt
		{
			cntRising[0] = ccr;									//Risisng Count
			TIM_OC1PolarityConfig(_TIMx,TIM_ICPolarity_Falling);	//Set Next Interrupt = Falling
		}		
		else                          				//Falling edge interrupt
		{
			if(ccr >= cntRising[0])							//Within One Period: 
				_IPC_Count[0] = ccr - cntRising[0];	//
			else																//Crosse Two Period:
				_IPC_Count[0] = _Period - cntRising[0] + ccr;
			TIM_OC1PolarityConfig(_TIMx,TIM_ICPolarity_Rising);		//Set Next Interrupt = Rising
		}
		isRising[0] = !isRising[0];						//Inverse Edge Flag
	}
	
	//Channel 2
	if(TIM_GetITStatus(_TIMx, TIM_IT_CC2) != RESET)						//Check Capture Interrupt Bit
	{
		ccr = TIM_GetCapture2(_TIMx);					//Get Capture Count
		if(isRising[1]) 		   								//Rising edge interrupt
		{
			cntRising[1] = ccr;									//Risisng Count
			TIM_OC2PolarityConfig(_TIMx,TIM_ICPolarity_Falling);	//Set Next Interrupt = Falling
		}		
		else                          				//Falling edge interrupt
		{
			if(ccr >= cntRising[1])							//Within One Period: 
				_IPC_Count[1] = ccr - cntRising[1];	//
			else																//Crosse Two Period:
				_IPC_Count[1] = _Period - cntRising[1] + ccr;
			TIM_OC2PolarityConfig(_TIMx,TIM_ICPolarity_Rising);		//Set Next Interrupt = Rising
		}
		isRising[1] = !isRising[1];						//Inverse Edge Flag
	}
	
	
	//Channel 3
	if(TIM_GetITStatus(_TIMx, TIM_IT_CC3) != RESET)						//Check Capture Interrupt Bit
	{
		ccr = TIM_GetCapture3(_TIMx);					//Get Capture Count
		if(isRising[2]) 		   								//Rising edge interrupt
		{
			cntRising[2] = ccr;									//Risisng Count
			TIM_OC3PolarityConfig(_TIMx,TIM_ICPolarity_Falling);	//Set Next Interrupt = Falling
		}		
		else                          				//Falling edge interrupt
		{
			if(ccr >= cntRising[2])							//Within One Period: 
				_IPC_Count[2] = ccr - cntRising[2];	//
			else																//Crosse Two Period:
				_IPC_Count[2] = _Period - cntRising[2] + ccr;
			TIM_OC3PolarityConfig(_TIMx,TIM_ICPolarity_Rising);		//Set Next Interrupt = Rising
		}
		isRising[2] = !isRising[2];						//Inverse Edge Flag
	}
	
	//Channel 4
	if(TIM_GetITStatus(_TIMx, TIM_IT_CC4) != RESET)						//Check Capture Interrupt Bit
	{
		ccr = TIM_GetCapture4(_TIMx);					//Get Capture Count
		if(isRising[3]) 		   								//Rising edge interrupt
		{
			cntRising[3] = ccr;									//Risisng Count
			TIM_OC4PolarityConfig(_TIMx,TIM_ICPolarity_Falling);	//Set Next Interrupt = Falling
		}		
		else                          				//Falling edge interrupt
		{
			if(ccr >= cntRising[3])							//Within One Period: 
				_IPC_Count[3] = ccr - cntRising[3];	//
			else																//Crosse Two Period:
				_IPC_Count[3] = _Period - cntRising[3] + ccr;
			TIM_OC4PolarityConfig(_TIMx,TIM_ICPolarity_Rising);		//Set Next Interrupt = Rising
		}
		isRising[3] = !isRising[3];						//Inverse Edge Flag
	}
	
}



void Timer::EncoderIRQ()
{
	if(TIM_GetITStatus(_TIMx, TIM_IT_Update) != RESET) //Check Update IT flag
	{
		TIM_ClearITPendingBit(_TIMx, TIM_IT_Update);		//Clear Update IT flag
		u16 cnt = TIM_GetCounter(_TIMx);								//Read current CNT count
		if(cnt<30000)																		//Count Up Overflow
			_EncoderPulse += 60000;												//Positive diraction, add one period
		else																						//Count Down Overflow
			_EncoderPulse -= 60000;                       //Negtive diraction, minus one period
	}
}




void Timer::IRQ()
{
	switch(_TIM_MODE)//Check Mode
	{
	case TIM_MODE_TIMER:		//Timer Mode
		if(TIM_GetITStatus(_TIMx, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(_TIMx, TIM_IT_Update);
			if(_UpdateIRQ==0)	//Check function pointer
				return;
			_UpdateIRQ();			//Update IRQ, user defined function
		}
		break;
	case TIM_MODE_IPC:		//IPC Mode
		InputCaptureIRQ();
		break;
	case TIM_MODE_COUNTER://Counter Mode
	case TIM_MODE_ENCODER://Encoder Mode
		EncoderIRQ();
		break;
	case TIM_MODE_PWM:		//PWM Mode, no interrupt
	default:							//Error, for safty, clear all Interrupt flag
		TIM_ClearITPendingBit(_TIMx, TIM_IT_CC1);
		TIM_ClearITPendingBit(_TIMx, TIM_IT_CC2);
		TIM_ClearITPendingBit(_TIMx, TIM_IT_CC3);
		TIM_ClearITPendingBit(_TIMx, TIM_IT_CC4);
		TIM_ClearITPendingBit(_TIMx, TIM_IT_Trigger);
		TIM_ClearITPendingBit(_TIMx, TIM_IT_Break);
		TIM_ClearITPendingBit(_TIMx, TIM_IT_Update);
		break;
	}
}

