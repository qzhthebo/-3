#include "stm32f10x.h"
#include "UART.h"
#include "Timer.h"
#include "EXIT.h"
#include "I2C.h"

extern "C"
{

//==============================================  EXTIx IRQHandler
void EXTI0_IRQHandler(void) 	  //----EXTI0 IRQ-----//
{ if(EXIT::pEXIT[0]) EXIT::pEXIT[0]->IRQ();}
void EXTI1_IRQHandler(void) 	  //----EXTI1 IRQ-----//
{	if(EXIT::pEXIT[1]) EXIT::pEXIT[1]->IRQ();}
void EXTI2_IRQHandler(void) 	  //----EXTI2 IRQ-----//
{	if(EXIT::pEXIT[2]) EXIT::pEXIT[2]->IRQ();}
void EXTI3_IRQHandler(void) 	  //----EXTI3 IRQ-----//
{	if(EXIT::pEXIT[3]) EXIT::pEXIT[3]->IRQ();}
void EXTI4_IRQHandler(void) 	  //----EXTI4 IRQ-----//
{	if(EXIT::pEXIT[4]) EXIT::pEXIT[4]->IRQ();}

void EXTI9_5_IRQHandler(void) 	//---EXTI9_5 IRQ----//
{
	if(EXTI_GetITStatus(EXTI_Line5))       //--EXTI5--//
	{		if(EXIT::pEXIT[5]) EXIT::pEXIT[5]->IRQ();	}
	if(EXTI_GetITStatus(EXTI_Line6))       //--EXTI6--//
	{		if(EXIT::pEXIT[6]) EXIT::pEXIT[6]->IRQ();	}	
	if(EXTI_GetITStatus(EXTI_Line7))       //--EXTI7--//
	{		if(EXIT::pEXIT[7]) EXIT::pEXIT[7]->IRQ();	}	
	if(EXTI_GetITStatus(EXTI_Line8))       //--EXTI8--//
	{		if(EXIT::pEXIT[8]) EXIT::pEXIT[8]->IRQ();	}
	if(EXTI_GetITStatus(EXTI_Line9))       //--EXTI9--//
	{		if(EXIT::pEXIT[9]) EXIT::pEXIT[9]->IRQ();	}
}

void EXTI15_10_IRQHandler(void) //---EXTI15_10 IRQ--//
{
	if(EXTI_GetITStatus(EXTI_Line10))     //--EXTI10--//
	{		if(EXIT::pEXIT[10]) EXIT::pEXIT[10]->IRQ();	}	
	if(EXTI_GetITStatus(EXTI_Line11))     //--EXTI11--//
	{		if(EXIT::pEXIT[11]) EXIT::pEXIT[11]->IRQ();	}
	if(EXTI_GetITStatus(EXTI_Line12))     //--EXTI12--//
	{		if(EXIT::pEXIT[12]) EXIT::pEXIT[12]->IRQ();	}
	if(EXTI_GetITStatus(EXTI_Line13))     //--EXTI13--//
	{		if(EXIT::pEXIT[13]) EXIT::pEXIT[13]->IRQ();	}
	if(EXTI_GetITStatus(EXTI_Line14))     //--EXTI14--//
	{		if(EXIT::pEXIT[14]) EXIT::pEXIT[14]->IRQ();	}
	if(EXTI_GetITStatus(EXTI_Line15))     //--EXTI15--//
	{		if(EXIT::pEXIT[15]) EXIT::pEXIT[15]->IRQ();	}
}


//==============================================  UARTx_IRQHandler 

void USART1_IRQHandler(void)    
{	if(UART::pUART[0]) 	UART::pUART[0]->IRQ();}
void USART2_IRQHandler(void)    
{	if(UART::pUART[1]) 	UART::pUART[1]->IRQ();}
void USART3_IRQHandler(void)   
{	if(UART::pUART[2]) 	UART::pUART[2]->IRQ();}
void UART4_IRQHandler(void)   
{	if(UART::pUART[3]) UART::pUART[3]->IRQ(); }
void UART5_IRQHandler(void)    
{	if(UART::pUART[4]) UART::pUART[4]->IRQ(); }



//==============================================  DMA  IRQHandler 

void DMA1_Channel1_IRQHandler() 
{     }
void DMA1_Channel2_IRQHandler() 
{	if(UART::pUART[2]) UART::pUART[2]->IRQ_TxDMA(); }
void DMA1_Channel3_IRQHandler() 
{  }
void DMA1_Channel4_IRQHandler() 
{	if(UART::pUART[0]) UART::pUART[0]->IRQ_TxDMA(); }
void DMA1_Channel5_IRQHandler() 
{  }
void DMA1_Channel6_IRQHandler()
{ }
void DMA1_Channel7_IRQHandler() 
{	if(UART::pUART[1]) UART::pUART[1]->IRQ_TxDMA(); }
              /*****DMA2*****/
void DMA2_Channel1_IRQHandler() 
{     }
void DMA2_Channel2_IRQHandler()
{     }
void DMA2_Channel3_IRQHandler() 
{ }
void DMA2_Channel4_5_IRQHandler()
{	if(UART::pUART[3]) UART::pUART[3]->IRQ_TxDMA(); }



//==============================================  TIMx  IRQHandler

void TIM1_UP_IRQHandler(void)   //----TIM1 Up-------//
{ if(Timer::pTIM[0]) Timer::pTIM[0]->IRQ();}
void TIM1_CC_IRQHandler(void)		//----TIM1 CC-------//
{	if(Timer::pTIM[0]) Timer::pTIM[0]->IRQ();}
void TIM2_IRQHandler(void)      //----TIM2 IRQ------// 
{ if(Timer::pTIM[1]) Timer::pTIM[1]->IRQ();}
void TIM3_IRQHandler(void)      //----TIM3 IRQ------// 
{	if(Timer::pTIM[2]) Timer::pTIM[2]->IRQ();}
void TIM4_IRQHandler(void)      //----TIM4 IRQ------// 
{	if(Timer::pTIM[3]) Timer::pTIM[3]->IRQ();}
void TIM5_IRQHandler(void)      //----TIM5 IRQ------// 
{	if(Timer::pTIM[4]) Timer::pTIM[4]->IRQ();}
void TIM6_IRQHandler(void)      //----TIM6 IRQ------// 
{	if(Timer::pTIM[5]) Timer::pTIM[5]->IRQ();}
void TIM7_IRQHandler(void)      //----TIM7 IRQ------// 
{	if(Timer::pTIM[6]) Timer::pTIM[6]->IRQ();}
void TIM8_UP_IRQHandler(void)   //----TIM8 Up-------//
{ if(Timer::pTIM[7]) Timer::pTIM[7]->IRQ();}
void TIM8_CC_IRQHandler(void)		//----TIM8 CC-------//
{	if(Timer::pTIM[7]) Timer::pTIM[7]->IRQ();}


//==============================================     I2Cx  IRQHandler 

void I2C1_EV_IRQHandler(void)   //----I2C1 EVENT----//
{	 if(I2C::pI2C[0])  I2C::pI2C[0]->EventIRQ();  }
void I2C1_ER_IRQHandler(void)   //----I2C1 ERROR----//
{	 if(I2C::pI2C[0])  I2C::pI2C[0]->ERRORIRQ();  }
void I2C2_EV_IRQHandler(void)   //----I2C2 EVENT----//
{	 if(I2C::pI2C[1])  I2C::pI2C[1]->EventIRQ();  }
void I2C2_ER_IRQHandler(void)   //----I2C2 ERROR----//
{	 if(I2C::pI2C[1])  I2C::pI2C[1]->ERRORIRQ();  }







}
