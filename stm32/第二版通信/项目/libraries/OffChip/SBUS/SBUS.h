#ifndef _SBUS_H_
#define _SBUS_H_

#include "stm32f10x.h"
#include "UART.h"


enum QUAD_FLIGHTMODE{
	DEBUG     = 0x00 ,
	NORMAL  	= 0x01 ,
	STOP      = 0x02 ,
	NONE_F		= 0x03
};

enum QUAD_SIGHTMODE{
	VIDEO 			= 0x00 ,
	STOP1 			= 0x01 ,
	PHOTO   		= 0x02 ,
	NONE_S			= 0x03
};


//Channel Definination
#define RC_ROLL 		0
#define RC_PITCH		1
#define RC_THROL		2
#define RC_YAW			3
#define FLIGHTMODE  4
#define SIGHTMODE		7
#define UPSIDEWAVE	8
#define UPSIDELEAN	9

#define SBUS_MIN		306
#define SBUS_MAX		1693



class SBUS{
private:
		UART &_com;
		FIFOBuffer<u8,500> &_Rx_Buffer;
public:
		SBUS(UART &com);
		void Update();			//update remotor data
public:
		u16 Channel_Origin[16];		//Channel Origin data
		u16 Channel[16];
		u8  Raw[25];				//SBUS Origin data
		bool tagOnline;			//Flag for Remotor online
		int cntUpdate;
public:
		u16 _ROLL_;
		u16 _PITCH_;
		u16 _THROL_;
		u16 _YAW_;
		QUAD_FLIGHTMODE  _FLIGHTMODE_;
		QUAD_SIGHTMODE   _SIGHTMODE_;
		u16 _UPSIDE_WAVE_;
		u16 _UPSIDE_LEAN_;
		
		u8 MtrEn;
		u8 RemotorUpdateFreq;
};





#endif
