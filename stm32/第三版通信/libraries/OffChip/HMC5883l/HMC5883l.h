#ifndef _HMC5883L_H_
#define _HMC5883L_H_

#include "stm32f10x.h"
#include "Compass.h"
#include "I2C.h"
#include "MathTools.h"

#define	HMC5883L_Addr   0x3C	 
#define HMC5883L_ConfigurationRegisterA  0x00
#define HMC5883L_ConfigurationRegisterB  0x01
#define HMC5883L_ModeRegister            0x02
#define HMC5883L_Output_X_MSB            0x03
#define HMC5883L_Output_X_LSB 					 0x04
#define HMC5883L_Output_Z_MSB            0x05
#define HMC5883L_Output_Z_LSB 					 0x06
#define HMC5883L_Output_Y_MSB            0x07
#define HMC5883L_Output_Y_LSB 					 0x08
#define HMC5883L_StatusRegister					 0x09
#define HMC5883L_ID_A										 0x0A
#define HMC5883L_ID_B 									 0x0B
#define HMC5883L_ID_C 									 0x0C



class HMC5883L :public Compass{
	
protected:
	I2C &_i2c;
	u8 _Sensor_data[6]; 
public:
	bool is_Success;
	HMC5883L(I2C &i2c);
	virtual bool Initialize();
	virtual u8 Update_data(Vector3f &magnet);
};






#endif
