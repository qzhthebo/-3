#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f10x.h"
#include "GPIO.h"

#define TIM_MODE_TIMER   0x01
#define TIM_MODE_PWM     0x02
#define TIM_MODE_IPC	 		0x03
#define TIM_MODE_ENCODER 0x04
#define TIM_MODE_FRQ     0x05
#define TIM_MODE_COUNTER 0x06

class Timer
{
protected:
	TIM_TypeDef* _TIMx;
	u8 _TIM_MODE;
	u16 _Period;
	u32 _IPC_Count[4];
	double _Resolution;
	int64_t _EncoderPulse;
public:
	static Timer* pTIM[8];
	int64_t _EncoderZero;
public:
	//Timer Mode
	Timer(u8 TimNumber,float ts,void (*FuncIRQ)()=0,u8 PrePri=3,u8 SubPri=0,u8 ItGroup=3);
	//Counter Mode
	Timer(u8 TimNumber,u16 ExtiTriggerPsc,bool remap=false);
	//PWM or Input Capture Mode
	Timer(u8 TimNumber,float FeqHz,bool isPWM,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4,bool remap=false,u8 PrePri=3,u8 SubPri=0,u8 ItGroup=3);
	//Encoder Mode
	Timer(u8 TimNumber,bool remap=false,u8 PrePri=3,u8 SubPri=0,u8 ItGroup=3);
	//Freqency variable Signal
	Timer(u8 TimNumber,bool remap,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4);

	void Enable(bool enable);

	void SetDutyRadio(u8 ChannelNumber,float duty);
	void SetDutyRadio(float channel1,float channel2,float channel3,float channel4);
	double InputCaptureValue(u8 ChannelNumber);
	int64_t Counts();
	void Encoder_IRQ();
	void SetHz(float hz);
protected:
	//inner calling
	void InitTimeBase(u8 TimNumber,float ts,u16 CountMode, bool isEncoder=false);
	void InitNVIC(IRQn irqChannel,u8 PrePri,u8 SubPri,u8 ItGroup);
	void InitGPIO(u8 TimNumber,GPIOMode_TypeDef mode,bool remap,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4);
	void InitOC(bool isPWM,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4);
	void InitIC(u16 IcEdge,bool EnCH1,bool EnCH2,bool EnCH3,bool EnCH4);
public:
	void IRQ();
protected:
	//Sub IRQ
	void (*_UpdateIRQ)();
	void InputCaptureIRQ();
	void EncoderIRQ();


	
};












#endif
