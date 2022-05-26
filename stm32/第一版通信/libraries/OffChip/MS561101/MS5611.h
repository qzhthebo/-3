#ifndef _MS_5611_H_
#define _MS_5611_H_

#include "stm32f10x.h"
#include "Barometer.h"
#include "I2C.h"


//#define MS5611_Pin_CSB_HIGH

#ifdef MS5611_Pin_CSB_HIGH
	#define MS5611_ADDR 0xEC
#else
	#define MS5611_ADDR 0xEE
#endif

//==================================    MS5611 Register
#define MS5611_D1 0x40			//Pressure
#define MS5611_D2 0x50			//Temperature
#define MS5611_RESET 0x1E
#define MS5611_D1D2_SIZE 3	// D1 and D2 result size (bytes)

// OSR (Over Sampling Ratio) constants
#define MS5611_OSR_256 0x00
#define MS5611_OSR_512 0x02
#define MS5611_OSR_1024 0x04
#define MS5611_OSR_2048 0x06
#define MS5611_OSR_4096 0x08

#define MS5611_PROM_BASE_ADDR 0xA2
// by adding ints from 0 to 6 we can read all the prom configuration values. 
// C1 will be at 0xA2 and all the subsequent are multiples of 2
#define MS5611_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS5611_PROM_REG_SIZE 2 // size in bytes of a prom registry.




class MS5611 :public Barometer{
protected:
	I2C &_mi2c;
	//Read Calibration data(factory calibrated) from PROM
	u8  _mRomData[12];
	u16 _mC[6];
	bool _mConvertPressure;
	//Temperature ADC value
	u8  _mRawT[3];
	//Pressure ADC value
	u8  _mRawP[3];

public:
	MS5611(I2C &i2c);
	virtual bool Initialize();
	virtual bool Update(float &pressure);
	float Temperature();
	float Pressure();
	bool IsSuccess;
	float toAltitude(float pressure);
	
	
};




#endif
