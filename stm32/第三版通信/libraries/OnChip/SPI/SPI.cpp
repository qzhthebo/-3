#include "SPI.h"


//Params for Speed at 72MHz
const u8 SPISpeed::MHz_36 		= SPI_BaudRatePrescaler_2;  
const u8 SPISpeed::MHz_18			= SPI_BaudRatePrescaler_4;   
const u8 SPISpeed::MHz_9 			= SPI_BaudRatePrescaler_8;   	
const u8 SPISpeed::MHz_4 			= SPI_BaudRatePrescaler_16;   	
const u8 SPISpeed::MHz_2 			= SPI_BaudRatePrescaler_32;   
const u8 SPISpeed::MHz_1 			= SPI_BaudRatePrescaler_64;  
const u8 SPISpeed::KHz_562		= SPI_BaudRatePrescaler_128;  
const u8 SPISpeed::KHz_281		= SPI_BaudRatePrescaler_256;  	

const u16 SPIDataSize::Bits_8	= SPI_DataSize_8b;
const u16 SPIDataSize::Bits_16	= SPI_DataSize_16b;




SPI::SPI(u8 SpiNumber,u8 speed,u8 dataSize,bool remap){
	Init_GPIO(SpiNumber,remap);
	Init_SPI(speed,dataSize);
	times = 0;
}




void SPI::Init_GPIO(u8 SpiNumber,bool remap){
	if(SpiNumber<1 || SpiNumber>3) return;
	
	SPI_TypeDef* tmpSPI[3] = {SPI1,SPI2,SPI3};
	_SPIx = tmpSPI[SpiNumber-1];
	
	if(SpiNumber==1 && remap){
		GPIO	_SCKPin(GPIOB,3,GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);
		GPIO  _MISOPin(GPIOB,4,GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);
		GPIO  _MOISPin(GPIOB,5,GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true); 
		GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
		
		_SCKPin=1 ;  _MISOPin=1;  _MOISPin=1;
	}else{
		//													SPI1     			SPI2				SPI3
		GPIO_TypeDef* SPIPort[3] = {GPIOA,       GPIOB,       GPIOB};
		u16         SCKPinNumber[3] = {5,           13,           3};   
		u16        MISOPinNumber[3] = {6,           14,           4};  
		u16        MOSIPinNumber[3] = {7,           15,           5};   
		GPIO  SCKPin(SPIPort[SpiNumber-1], SCKPinNumber[SpiNumber-1],GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);  
		GPIO MISOPin(SPIPort[SpiNumber-1],MISOPinNumber[SpiNumber-1],GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);  
		GPIO MOSIPin(SPIPort[SpiNumber-1],MOSIPinNumber[SpiNumber-1],GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);  
		
		SCKPin = 1;	 MISOPin = 1;	 MOSIPin = 1;		
	}
}


void SPI::Init_SPI(u8 speed,u8 dataSize){
	if(_SPIx == SPI1)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	else if(_SPIx==SPI2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	else
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_Cmd(_SPIx, DISABLE); 
	//double direction
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												
	SPI_InitStructure.SPI_DataSize = dataSize;										   
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;											
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;											
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;												
	SPI_InitStructure.SPI_BaudRatePrescaler = speed;									
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						
	SPI_InitStructure.SPI_CRCPolynomial = 7;							
	
	SPI_Init(_SPIx, &SPI_InitStructure);  
 
	SPI_Cmd(_SPIx, ENABLE); 
	
	//start communication
	//WriteBytes(0XFFFF);
}



void SPI::Init_NVIC(){
	
}

u8 SPI::ReadWriteByte(u8 data){
			/* Loop while DR register in not emplty */
		while(SPI_I2S_GetFlagStatus(_SPIx, SPI_I2S_FLAG_TXE) == RESET);

		/* Send byte through the SPI2 peripheral */
		SPI_I2S_SendData(_SPIx, data);

		/* Wait to receive a byte */
		while(SPI_I2S_GetFlagStatus(_SPIx, SPI_I2S_FLAG_RXNE) == RESET);

		/* Return the byte read from the SPI bus */
		return SPI_I2S_ReceiveData(_SPIx);  
}

