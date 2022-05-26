#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"
#include "GPIO.h"

class SPI{

public:
	SPI(u8 SpiNumber,u8 speed,u8 dataSize,bool remap=false);
	u8 ReadWriteByte(u8 data);
	u16 times;
private:
	SPI_TypeDef* _SPIx;

private:
	void Init_GPIO(u8 SpiNumber,bool remap=false);
	void Init_SPI(u8 speed,u8 dataSize);
	void Init_NVIC();
};


struct SPISpeed
{
public:
		static const u8 MHz_36;    //SPI_BaudRatePrescaler_2, 36 MHz at 72 MHz system clock
  	static const u8 MHz_18;    //SPI_BaudRatePrescaler_4, 18 MHz at 72 MHz system clock
  	static const u8 MHz_9;   	 //SPI_BaudRatePrescaler_8, 9 MHz at 72 MHz system clock
		static const u8 MHz_4;   	 //SPI_BaudRatePrescaler_16,4.5 MHz at 72 MHz system clock
		static const u8 MHz_2;   	 //SPI_BaudRatePrescaler_32,2.25 MHz at 72 MHz system clock
		static const u8 MHz_1;   	 //SPI_BaudRatePrescaler_64,1.125 MHz at 72 MHz system clock
    static const u8 KHz_562;   //SPI_BaudRatePrescaler_128,562.5 KHz at 72 MHz system clock
		static const u8 KHz_281;   //SPI_BaudRatePrescaler_256,281.2 KHz at 72 MHz system clock	
private:
	SPISpeed(){}
};

struct SPIDataSize
{
public:
	static const u16 Bits_8;
	static const u16 Bits_16;
private:
	SPIDataSize(){}
};



#endif
