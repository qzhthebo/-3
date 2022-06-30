
#ifndef __UART_H_
#define __UART_H_

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "GPIO.h"

#define USART_TX_BUFFER_SIZE     500
#define USART_RX_BUFFER_SIZE		 500
#define USART_DMA_TX_BUFFER_SIZE 300
#define USART_DMA_RX_BUFFER_SIZE 300
#define endl "\n"

class UART
{
protected:
	USART_TypeDef* _UART_T;

	//DMA OF Trasmit
	u8* _TXDMA_Buffer; 
	bool _TXDMA_EN;
	DMA_Channel_TypeDef* _TXDMA_Channel;
	u32 _TXDMA_Flag_TC;
	u32 _TXDMA_IT_GL;

	//DMA OF Receive
	u8* _RXDMA_Buffer;
	bool _RXDMA_EN;
	DMA_Channel_TypeDef* _RXDMA_Channel;
	u32 _RXDMA_Flag_TC;
	u32 _RXDMA_IT_GL;

	bool _IsBusyToSend;
	FIFOBuffer<u8,USART_RX_BUFFER_SIZE> _TX_Buffer;
	u16 _TX_OverFlowNumber;
	u16 _RX_OverFlowNumber;
	u8 _Precision;
	
public:
	FIFOBuffer<u8,USART_RX_BUFFER_SIZE> _RX_Buffer;
	static UART* pUART[5];
public:
	UART(u8 uartnumber,u32 baud,bool remap=false,bool TXDMA_EN=true,bool RXDMA_EN=true,u16 parity=USART_Parity_No,u16 wordLen=USART_WordLength_8b, u16 stopBits=USART_StopBits_1);
	void TXDMA_Init(u8 uartnumber);
	void RXDMA_Init(u8 uartnumber);

	bool SendBytes(u8 TXData[],u16 size);
	bool SendByte(u8 data);
	bool GetBytes(u8 data[],u16 number);
	bool GetByte(u8 &data);
	u16 Txbuffer_Size();
	u16 Rxbuffer_Size();
	void FlushTxBuffer();
	void FlushRxBuffer();
	u16 TX_OverFlowNumber();
	u16 RX_OverFlowNumber();

	UART& operator<<(int64_t value);
	UART& operator<<(int32_t value);
	UART& operator<<(u32 	 value);
	UART& operator<<(double  value);
	UART& operator<<(const char* String);

	void Precision(u8 precision){ _Precision = precision;}
	void IRQ();
	void IRQ_TxDMA();
	//void IRQ_RxDMA();

};




#endif
