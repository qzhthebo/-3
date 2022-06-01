#include "ADC.h"
#include "GPIO.h"

u16 ADC::_ADC_Value[18] = {0};
u8  ADC::_ADC_Channel_List[18] = {0};
u8  ADC::_ADC_Channel_Count = 0;



ADC::ADC(u8 ChannelNumber,float k){
	if(ChannelNumber>=18) return;
	_k = k;
	_Order_Number = _ADC_Channel_Count;
	_ADC_Channel_List[_Order_Number] = ChannelNumber;
	_ADC_Channel_Count++;
	_ADC_Channel_Number = ChannelNumber;
	ADC::Vol = 11;
	if(ChannelNumber <= 15){
		//                         AIN0   AIN1  AIN2  AIN3  AIN4  AIN5  AIN6  AIN7  AIN8  AIN9 AIN10 AIN11 AIN12 AIN13 AIN14 AIN15 
		GPIO_TypeDef*  _PORT[16] = {GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOB,GPIOB,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC};
		u8			   _PIN[16]  = {0,    1,     2,    3,    4,    5,    6,    7,    0,    1,    0,    1,    2,    3,    4,    5};	
		GPIO ADC_PIN(_PORT[ChannelNumber],_PIN[ChannelNumber],GPIO_Mode_AIN,GPIO_Speed_50MHz,true);
	}
}


void ADC::InitDMA(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = u32(&(ADC1->DR));				
	DMA_InitStructure.DMA_MemoryBaseAddr = u32(_ADC_Value);	                   
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;												
	DMA_InitStructure.DMA_BufferSize = _ADC_Channel_Count;                             
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	         
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			    				
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;       
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		                        						
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                       
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                               
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);            									
	DMA_Cmd(DMA1_Channel1, ENABLE);                         					

}


void ADC::InitADC(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;											
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 						
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						
	ADC_InitStructure.ADC_NbrOfChannel = _ADC_Channel_Count;		
	ADC_Init(ADC1,&ADC_InitStructure);		
	
	for(int i=0;i<_ADC_Channel_Count;i++){
		ADC_RegularChannelConfig(ADC1,_ADC_Channel_List[i],i+1,ADC_SampleTime_239Cycles5);
		if(_ADC_Channel_List[i] == 16)
			ADC_TempSensorVrefintCmd(ENABLE);
	}
	ADC_DMACmd(ADC1,ENABLE);                   
	ADC_Cmd(ADC1,ENABLE);                 
	ADC_ResetCalibration(ADC1);					
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);				
	while(ADC_GetCalibrationStatus(ADC1));		
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);    

}



void ADC::Start(){
	InitDMA();
	InitADC();
}

float ADC::Value(){
	//voltage = Raw data * resolusion * coefficient
	float Voltage = _ADC_Value[_Order_Number]*0.0008056640625*_k;
	//resolusion = 3.3v / 2^12 = 3.3 / 4096 = 0.0008056640625
	if(_ADC_Channel_Number==16)  //Internal Temperature channel
	{
		float temperature = (1.43-Voltage)/0.0043+25; //Calculate Temperature
		return temperature;   //return temperature
	}
	else
		return Voltage;
}
int ADC::Is_LowVol(){
	return (ADC::Vol<9.5);
}

void ADC::Updata(int cnt){
	static float Vol_cnt=0;
	ADC::adc_cnt++;
	Vol_cnt += ADC::Value()*11;					
		if(adc_cnt>=cnt){
		adc_cnt = 0;Vol_cnt = 0;						
		ADC::Vol = Vol_cnt/cnt;
		}
}


