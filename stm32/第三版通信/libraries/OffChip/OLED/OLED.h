#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f10x.h"
#include "SPI.h"
#include "GPIO.h"
#include "OLEDFont.h"



// 0.96 OLED
/*
	GND 				OLED电源地
	VCC					OLED电源正(3.3V~5V)
	D0					OLED SPI和IIC总线时钟
	D1					OLED总线数据信号 MOSI
	RES					OLED复位信号,低电平复位(IIC时需要接高电平)
	DC					OLED命令/数据选择信号 
							高电平:数据
							低电平:命令(3线SPI时可以不接),IIC时需要接地
	CS					OLED片选信号,低电平使能, IIC总线时需要接电源地
*/
#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

class OLED {
protected:
		SPI &_mSPI;
private:
		GPIO _RES;
		GPIO _DC;
		GPIO _CS;
		//virtual display memory
		u8 OLED_GRAM[144][8];

public:
		OLED(SPI &_SPIx,GPIO &RES, GPIO &DC,GPIO &CS);

		void OLED_ClearPoint(u8 x,u8 y);
		void OLED_ColorTurn(bool isNormal); 
		void OLED_DisplayTurn(bool isNormal);			
		void OLED_WriteByte(u8 data,u8 mode);		
		void OLED_DisPlay_On(void);
		void OLED_DisPlay_Off(void);
		void OLED_Refresh(void);
		void OLED_Clear(void);
		void OLED_DrawPoint(u8 x,u8 y,u8 t);
		void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
		void OLED_DrawCircle(u8 x,u8 y,u8 r);
		void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
		void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
		void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
		void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
		void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
		void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
		void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
		void debug();
};



#endif
