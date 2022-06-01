#include "UART.h"

UART* UART::pUART[5]={0,0,0,0,0};
		

UART::UART(u8 uartnumber,u32 baud,bool remap,bool TXDMA_EN,bool RXDMA_EN,u16 parity,u16 wordLen, u16 stopBits)
{
	_TXDMA_EN = ((uartnumber<=4)? TXDMA_EN : false);
	_RXDMA_EN = ((uartnumber<=4)? RXDMA_EN : false);

	USART_TypeDef* tmpUART[5]={USART1,USART2,USART3,(USART_TypeDef*)(PERIPH_BASE+0x4C00),(USART_TypeDef*)(PERIPH_BASE+0x5000)};
	_UART_T = tmpUART[uartnumber-1];

	//Initialize GPIO
	if(uartnumber<=3 && remap){
		//																USART1     		USART2      USART3 
		GPIO_TypeDef* Tx_Port[3] 	 	  = { GPIOB,				GPIOD,      GPIOD};
		u16           Tx_PinNumber[3] = {   6,           5,           8  };
		GPIO_TypeDef* Rx_Port[3] 	    = { GPIOB,				GPIOD,      GPIOD};
		u16           Rx_PinNumber[3] = {   7,           6,           9  };
		GPIO Tx_Pin(Tx_Port[uartnumber-1],Tx_PinNumber[uartnumber-1],GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);
		GPIO Rx_Pin(Rx_Port[uartnumber-1],Rx_PinNumber[uartnumber-1],GPIOMode::IN_FLOATING,GPIOSpeed::HIGH,true); 
		u32 RemapConfig[3] = {GPIO_Remap_USART1,GPIO_Remap_USART2,GPIO_FullRemap_USART3};
		GPIO_PinRemapConfig(RemapConfig[uartnumber-1],ENABLE);

	}else{
		//															USART1     USART2      USART3 		USART4      USART5
		GPIO_TypeDef* Tx_Port[5] 	  =  { GPIOA,		GPIOA,      GPIOB,      GPIOC,      GPIOC};
		u16           Tx_PinNumber[5]= {   9,      2,           10,        10,          12 };
		GPIO_TypeDef* Rx_Port[5] 	  =  { GPIOA,		GPIOA,      GPIOB,      GPIOC,      GPIOD};
		u16           Rx_PinNumber[5]= {   10,      3,          11,        11,          2 };	
		GPIO Tx_Pin(Tx_Port[uartnumber-1],Tx_PinNumber[uartnumber-1],GPIOMode::OUT_PP_AF,GPIOSpeed::HIGH,true);
		GPIO Rx_Pin(Rx_Port[uartnumber-1],Rx_PinNumber[uartnumber-1],GPIOMode::IN_FLOATING,GPIOSpeed::HIGH,true); 	
	}

	//Initialize NVIC
	//u32 tmpGroup[5] = {NVIC_PriorityGroup_0,NVIC_PriorityGroup_1,NVIC_PriorityGroup_2,NVIC_PriorityGroup_3,NVIC_PriorityGroup_4};
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	//						 USART1        USART2     USART3    USART4    USART5
	IRQn_Type tmpIQRn[5] = {USART1_IRQn, USART2_IRQn, IRQn(39), IRQn(52), IRQn(53)};
	NVIC_InitTypeDef NVIC_InitStructure;  
	NVIC_InitStructure.NVIC_IRQChannel                   = tmpIQRn[uartnumber-1];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 

	if(_TXDMA_EN){
		//														UART1               UART2              UART3         UART4(DMA2_Ch4_5)		
		IRQn_Type tmpIRQn_DMA[4] = {DMA1_Channel4_IRQn, DMA1_Channel7_IRQn, DMA1_Channel2_IRQn,  IRQn(59)};
		NVIC_InitStructure.NVIC_IRQChannel = tmpIRQn_DMA[uartnumber-1];
		NVIC_Init(&NVIC_InitStructure); 
		//TXDMA_Init(uartnumber);
	}
//	if(_RXDMA_EN){
//		//							UART1               UART2              UART3         UART4(DMA2_Ch4_5)		
//		IRQn_Type tmpIRQn_DMA[4] = {DMA1_Channel5_IRQn, DMA1_Channel6_IRQn, DMA1_Channel3_IRQn,  IRQn(58)};
//		NVIC_InitStructure.NVIC_IRQChannel = tmpIRQn_DMA[uartnumber-1];
//		NVIC_Init(&NVIC_InitStructure); 
//		RXDMA_Init(uartnumber);
//	}

	//Initialize UART
	//       UART RCC:         UART1                UART2                 UART3                 UART4                  UART5
	u32 RCC_UART[5] = {RCC_APB2Periph_USART1, RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_UART4, RCC_APB1Periph_UART5}; 
	if(uartnumber == 1) RCC_APB2PeriphClockCmd(RCC_UART[uartnumber-1],ENABLE);
	else 								RCC_APB1PeriphClockCmd(RCC_UART[uartnumber-1],ENABLE);
	USART_InitTypeDef USART_InitStructure;    
	USART_InitStructure.USART_BaudRate            = baud;     
	USART_InitStructure.USART_WordLength          = wordLen;  
	USART_InitStructure.USART_StopBits            = stopBits; 
	USART_InitStructure.USART_Parity              = parity ; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(_UART_T, &USART_InitStructure);  
	USART_Cmd(_UART_T, ENABLE);                
	//UART Interrupt Config
	if(_RXDMA_EN){
		USART_ITConfig(_UART_T, USART_IT_RXNE, DISABLE);
		USART_ITConfig(_UART_T, USART_IT_IDLE, ENABLE);	
	}
	else
		USART_ITConfig(_UART_T, USART_IT_RXNE, ENABLE);

	if(_TXDMA_EN)  
			USART_ITConfig(_UART_T,USART_IT_TC,DISABLE);

	USART_ClearFlag(_UART_T, USART_FLAG_TC);
	USART_ClearFlag(_UART_T, USART_IT_RXNE);
	USART_ClearFlag(_UART_T, USART_IT_IDLE);

	if(_RXDMA_EN)	
	{
		_RXDMA_Buffer = new u8[USART_DMA_RX_BUFFER_SIZE];
		RXDMA_Init(uartnumber);
	}
	
	if(_TXDMA_EN)
	{
		_TXDMA_Buffer = new u8[USART_DMA_TX_BUFFER_SIZE];
		TXDMA_Init(uartnumber);
	}
	pUART[uartnumber-1] = this;

	_Precision = 5;
	
	
}
//Initialize DMA Tx Channel
void UART::TXDMA_Init(u8 uartnumber){
	if(uartnumber<4) RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	else			 			 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

//	FIFOBuffer<u8,USART_RX_BUFFER_SIZE> test;
//	test = new u8[USART_DMA_TX_BUFFER_SIZE];

//	_TXDMA_Buffer = new u8[USART_DMA_TX_BUFFER_SIZE];

	
	
	
	//																					 UART1           UART2          UART3           UART4
	DMA_Channel_TypeDef* tmp_DMAChannel[4] = {DMA1_Channel4, DMA1_Channel7, DMA1_Channel2,  DMA2_Channel5};
	_TXDMA_Channel = tmp_DMAChannel[uartnumber-1];
	//       					 UART1            UART2         UART3          UART4
	u32 tmpDMATCFlag[4] = {DMA1_FLAG_TC4, DMA1_FLAG_TC7, DMA1_FLAG_TC2, DMA2_FLAG_TC5};
	_TXDMA_Flag_TC = tmpDMATCFlag[uartnumber-1];
	//       			UART1            UART2         UART3          UART4
	u32 tmpDMAITGL[4] = {DMA1_IT_GL4, DMA1_IT_GL7, DMA1_IT_GL2, DMA2_IT_GL5};
	_TXDMA_IT_GL = tmpDMAITGL[uartnumber-1];

	DMA_DeInit(_TXDMA_Channel);  
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&_UART_T->DR;	      
	DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)_TXDMA_Buffer;         
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;	
	DMA_InitStructure.DMA_BufferSize         = 0;                      
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal ;       
	DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;        
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;        
	DMA_Init(_TXDMA_Channel, &DMA_InitStructure);    
		
	//DMA Interrupt Config
	DMA_ITConfig(_TXDMA_Channel, DMA_IT_TC, ENABLE);   
	DMA_ITConfig(_TXDMA_Channel, DMA_IT_TE, ENABLE);   
	USART_DMACmd(_UART_T, USART_DMAReq_Tx, ENABLE);          
	DMA_Cmd (_TXDMA_Channel,DISABLE);	
}

void UART::RXDMA_Init(u8 uartnumber){
	if(uartnumber<4) RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	else			 			 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	
//	_RXDMA_Buffer = new u8[USART_DMA_RX_BUFFER_SIZE];
	
	//											 UART1           UART2          UART3           UART4
	DMA_Channel_TypeDef* tmp_DMAChannel[4] = {DMA1_Channel5, DMA1_Channel6, DMA1_Channel3,  DMA2_Channel3};
	_RXDMA_Channel = tmp_DMAChannel[uartnumber-1];
	//       					 UART1            UART2         UART3          UART4
	u32 tmpDMATCFlag[4] = {DMA1_FLAG_TC5, DMA1_FLAG_TC6, DMA1_FLAG_TC3, DMA2_FLAG_TC3};
	_RXDMA_Flag_TC = tmpDMATCFlag[uartnumber-1];
	//       			UART1            UART2         UART3          UART4
	u32 tmpDMAITGL[4] = {DMA1_IT_GL5, DMA1_IT_GL6, DMA1_IT_GL3, DMA2_IT_GL3};
	_RXDMA_IT_GL = tmpDMAITGL[uartnumber-1];

	DMA_DeInit(_RXDMA_Channel);  
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&_UART_T->DR;	      
	DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)_RXDMA_Buffer;         
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;	
	DMA_InitStructure.DMA_BufferSize         = USART_DMA_RX_BUFFER_SIZE;                      
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal ;       
	DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;        
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;        
	DMA_Init(_RXDMA_Channel, &DMA_InitStructure);    
		
	//DMA Interrupt Config
	//DMA_ITConfig(_RXDMA_Channel, DMA_IT_TC, ENABLE);   
	//DMA_ITConfig(_RXDMA_Channel, DMA_IT_TE, ENABLE);   
	USART_DMACmd(_UART_T, USART_DMAReq_Rx, ENABLE);          
	DMA_Cmd (_RXDMA_Channel,ENABLE);		
}

//=====================================================   Send Bytes
bool UART::SendBytes(u8 TXData[],u16 size){
	if(_TX_Buffer.RestOfSize()<size){
		_TX_OverFlowNumber += size;
		return false;
	}
	
	_TX_Buffer.PutDatas(TXData,size);
	if(_IsBusyToSend)    return true;
	if(_TX_Buffer.size()<=0) return true;
	_IsBusyToSend = true;
	
	if(_TXDMA_EN){
		// DMA transmit
		if(_TX_Buffer.size()<=USART_DMA_TX_BUFFER_SIZE){
			_TXDMA_Channel->CNDTR = (u32)_TX_Buffer.size();
			_TX_Buffer.GetDatas(_TXDMA_Buffer,_TX_Buffer.size());
		}else{
			_TXDMA_Channel->CNDTR = USART_DMA_TX_BUFFER_SIZE;
			_TX_Buffer.GetDatas(_TXDMA_Buffer,USART_DMA_TX_BUFFER_SIZE);
		}
		DMA_Cmd(_TXDMA_Channel,ENABLE);
	}else{
		// Interrupt trasmit
		USART_ClearITPendingBit(_UART_T,USART_IT_TC);
		USART_ClearITPendingBit(_UART_T,USART_IT_TXE);
		USART_ITConfig(_UART_T,USART_IT_TC,ENABLE);
		USART_GetFlagStatus(_UART_T,USART_FLAG_TC);
		static u8 data=0;
		_TX_Buffer.GetData(data);
		USART_SendData(_UART_T,data);
	}
	return true;
	
}

//=====================================================   Send Byte
bool UART::SendByte(u8 data){
	if(_TX_Buffer.RestOfSize()<1){
		_TX_OverFlowNumber++;
		return false;
	}
	_TX_Buffer.PutData(data);
	if(_IsBusyToSend)         return true;
	if(_TX_Buffer.size()<=0)  return true;
	_IsBusyToSend = true;
	
	if(_TXDMA_EN){
		// DMA Transmit
		if(_TX_Buffer.size()<=USART_DMA_TX_BUFFER_SIZE){
			_TXDMA_Channel->CNDTR = (u32)_TX_Buffer.size();
			_TX_Buffer.GetDatas(_TXDMA_Buffer,_TX_Buffer.size());
		}else{
			_TXDMA_Channel->CNDTR = USART_DMA_TX_BUFFER_SIZE;
			_TX_Buffer.GetDatas(_TXDMA_Buffer,USART_DMA_TX_BUFFER_SIZE);
		}
		DMA_Cmd(_TXDMA_Channel,ENABLE);
	}else{
		// Interrupt Transmit
		USART_ClearITPendingBit(_UART_T,USART_IT_TC);
		USART_ClearITPendingBit(_UART_T,USART_IT_TXE);
		USART_ITConfig(_UART_T,USART_IT_TC,ENABLE);
		USART_GetFlagStatus(_UART_T,USART_FLAG_TC);
		static u8 data=0;
		_TX_Buffer.GetData(data);
		USART_SendData(_UART_T,data);
	}
	return true;
}

//=====================================================   Get Bytes
bool UART::GetBytes(u8 data[],u16 number){
	return _RX_Buffer.GetDatas(data,number);	
}

//=====================================================   Get Byte
bool UART::GetByte(u8 &data){
	return _RX_Buffer.GetData(data);	
}

//=====================================================   Get RxBuffer Size
u16 UART::Rxbuffer_Size(){
	return _RX_Buffer.size();
}


//=====================================================   Get TxBuffer Size
u16 UART::Txbuffer_Size(){
	return _TX_Buffer.size();
}

void UART::FlushTxBuffer(){
	_TX_Buffer.flush();
}
void UART::FlushRxBuffer(){
	_RX_Buffer.flush();
}

//=====================================================   Get TX Overflow Bytes number
u16 UART::TX_OverFlowNumber(){
	return _TX_OverFlowNumber;
}

//=====================================================   Get RX Overflow Bytes number
u16 UART::RX_OverFlowNumber(){
	return _RX_OverFlowNumber;
}


//=====================================================   Send Int number with ASCII
UART& UART::operator<<(int64_t value){
	u8 sign=0,len=0,data[30];
	if(value<0){
		sign=1;
		value = -value;
	}
	
	do{
		len++;
		data[10-len] = value%10 + '0';
		value=value/10;
	}while(value);
	
	if(sign==1)  
		data[10-(++len)] = '-';
	SendBytes(data+10-len,len);
	return *this;
}

//=====================================================   Send Int number with ASCII
UART& UART::operator<<(int32_t value){
	u8 sign=0,len=0,data[20];
	if(value<0){
		sign=1;
		value = -value;
	}
	
	do{
		len++;
		data[20-len] = value%10 + '0';
		value=value/10;
	}while(value);
	
	if(sign==1)  
		data[20-(++len)] = '-';
	SendBytes(data+20-len,len);
	return *this;	
	
}

//=====================================================   Send Int number with ASCII
UART& UART::operator<<(u32 	 value){
	u8 len=0,data[20];
	
	do{
		len++;
		data[20-len] = value%10 + '0';
		value=value/10;
	}while(value);
	
	SendBytes(data+20-len,len);
	return *this;
}

//=====================================================   Send Double number with ASCII
UART& UART::operator<<(double  value){
	u8 sign=0,len=0,data[20];
	if(value<0){
		sign=1;
		value = -value;
	}
	// make it to int
	if(_Precision>0){
		u8 prec = _Precision;
		while(prec--)
			value *= 10; 
	}
	u32 value_ = value;
	
	while(1){
		len++;
		if(len == _Precision+1 && _Precision >0)
			data[20-len] = '.';   //add dot
		else{
			data[20-len] = value_%10 +'0';
			value_=value_/10;
		}
		if(value_ == 0){
			if(_Precision==0)
				break;
			else if(len >= _Precision+2)
				break;
		}
	}
	if(sign==1)
		data[20-(++len)] = '-';
	SendBytes(data+20-len,len);
	return *this;
}


//=====================================================   Send String

UART& UART::operator<<(const char* String){
	unsigned int length=0;
	for(int i=0;String[i]!='\0';++i){
		++length;
	}
	SendBytes((u8*)String,length);
	return *this;
}


//=====================================================   UART IRQ   =====================================================// 
void UART::IRQ(){
	if(USART_GetFlagStatus(_UART_T,USART_FLAG_ORE)!=RESET){
		USART_ReceiveData(_UART_T);
	}
	
	//=============================  RX ==================================//
	if(!_RXDMA_EN){
		// Interrupt Rx
		if(USART_GetITStatus(_UART_T,USART_IT_RXNE)!=RESET){
			USART_ClearITPendingBit(_UART_T,USART_IT_RXNE);
			if(!_RX_Buffer.PutData(USART_ReceiveData(_UART_T))){
				_RX_OverFlowNumber++;
			}
		}
	}else{
		// DMA Rx
		if(USART_GetITStatus(_UART_T,USART_IT_IDLE)!=RESET){
			u16 bytecount = _UART_T->SR;
			bytecount = _UART_T->DR;
			DMA_ClearITPendingBit(_RXDMA_IT_GL);
			DMA_ClearFlag(_RXDMA_Flag_TC);
			DMA_Cmd(_RXDMA_Channel,DISABLE);
			
			bytecount = USART_DMA_RX_BUFFER_SIZE - DMA_GetCurrDataCounter(_RXDMA_Channel);
			if(bytecount>0 && bytecount<USART_DMA_RX_BUFFER_SIZE)
				if(!_RX_Buffer.PutDatas(_RXDMA_Buffer,bytecount)){
					_RX_OverFlowNumber += bytecount;
				}
			DMA_SetCurrDataCounter(_RXDMA_Channel,USART_DMA_RX_BUFFER_SIZE);
			DMA_Cmd(_RXDMA_Channel,ENABLE);
		}
	}
	
	
	//=============================  TX ==================================//
	if(!_TXDMA_EN){
		if(USART_GetITStatus(_UART_T,USART_IT_TXE)!=RESET || USART_GetITStatus(_UART_T,USART_IT_TC)!=RESET){
			u8 data=0;
			if(_TX_Buffer.size()>0){
				_TX_Buffer.GetData(data);
				//Send one Byte
				_UART_T->DR = (data & (u16)0x01FF);
			}else{
				USART_ITConfig(_UART_T,USART_IT_TXE,DISABLE);
				USART_ITConfig(_UART_T,USART_IT_TC,DISABLE);
				_IsBusyToSend = false;
			}
		}
	}
}
		
	
//=====================================================   TxDMA IRQ   =====================================================//	
void UART::IRQ_TxDMA(){
	if(!_TXDMA_EN) return;
	
	if(DMA_GetFlagStatus(_TXDMA_Flag_TC)==SET){
		DMA_ClearITPendingBit(_TXDMA_IT_GL);
		DMA_ClearFlag(_TXDMA_Flag_TC);
		DMA_Cmd(_TXDMA_Channel,DISABLE);
		
		if(_TX_Buffer.size()>0){
			if(_TX_Buffer.size() <= USART_DMA_TX_BUFFER_SIZE){
				_TXDMA_Channel->CNDTR = _TX_Buffer.size();
				_TX_Buffer.GetDatas(_TXDMA_Buffer,_TX_Buffer.size());
			}else{
				_TXDMA_Channel->CNDTR = USART_DMA_TX_BUFFER_SIZE;
				_TX_Buffer.GetDatas(_TXDMA_Buffer,USART_DMA_TX_BUFFER_SIZE);				
			}
			DMA_Cmd(_TXDMA_Channel,ENABLE);
		}else
			_IsBusyToSend = false;
	}
}

// Rx DMA IRQ 
//void UART::RxDMAIRQ()
//{
//	static u16 i = 0;
//	
//	if(++i>10){i=0;
//	ledSys = !ledSys.Get();
//	}
//	//Check whether Enable DMA for UART Rx
//	if(!_enRxDMA) return;  //if not, exit
//	
//	//Check DMA TC Flag
//	if(DMA_GetFlagStatus(_flagRxDMATC)==SET) //DMA Tx Complete
//	{
//		DMA_ClearITPendingBit(_itRxDMAGL);//Clear DMA global interrupt flag
//		DMA_ClearFlag(_flagRxDMATC);      //Clear DMA Tx complete flag
//		DMA_Cmd(_rxDMACh,DISABLE);        //Close DMA
//		if(!_rxBuf.Puts(_pRxDMABuf, RxDMACnt))//Read data from Rx DMA buffer
//		{
//			_rxOverflowNum += RxDMACnt;
//		}
//		_rxDMACh->CNDTR = RxDMACnt;
//		DMA_Cmd(_rxDMACh,ENABLE); //enable DMA
//	}
//}

