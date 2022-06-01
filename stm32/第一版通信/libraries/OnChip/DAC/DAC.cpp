#include "DAC.h"


CDAC::CDAC(u8 ChannelNumber){
	if(ChannelNumber!=1 && ChannelNumber!=2)
		return;
	_DAC_Channel_Number = ChannelNumber;


	if(ChannelNumber==1){
		GPIO DACPin(GPIOA,4,GPIO_Mode_AIN,GPIO_Speed_50MHz,true);
		DACPin.Set(true);
	}else if(ChannelNumber==2){
		GPIO DACPin(GPIOA,5,GPIO_Mode_AIN,GPIO_Speed_50MHz,true);
		DACPin.Set(true);
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	DAC_InitTypeDef DAC_InitType;
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;												
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;				
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable;	
	if(chNum==1)		
	{
		DAC_Init(DAC_Channel_1,&DAC_InitType);				
		DAC_Cmd(DAC_Channel_1, ENABLE);							
		DAC_SetChannel1Data(DAC_Align_12b_R, 0);			
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE); 
	}
	else						
	{
		DAC_Init(DAC_Channel_2,&DAC_InitType);				
		DAC_Cmd(DAC_Channel_2, ENABLE);								
		DAC_SetChannel2Data(DAC_Align_12b_R, 0);		
		DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE); 
	}	

}



void CDAC::SetVoltage(float voltage){

	if(voltage<0 || voltage>3.3)  	
		return;
	
	float vol = voltage*4096/3.3f;	
	if(_DAC_Channel_Number==1)								
	{
		DAC_SetChannel1Data(DAC_Align_12b_R,vol);			
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE); 
	}
	else if(_DAC_Channel_Number==2)						
	{
		DAC_SetChannel2Data(DAC_Align_12b_R,vol);			
		DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE); 
	}		
}

