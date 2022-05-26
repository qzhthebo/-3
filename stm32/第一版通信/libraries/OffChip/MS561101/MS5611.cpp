#include "MS5611.h"
#include <math.h>

MS5611::MS5611(I2C &i2c):_mi2c(i2c){
	Initialize();
}

// 1 mbar = 0.1 Kpa

// Maximum Values for calculation results:
// Pmin = 10mbar   Pmax = 1200mbar
// Tmin = -40°„C  Tmax = 85°„C  Tref = 20°„C 

/*
	Reset
	Read PROM(128 bit of calibration words)
	D1 conversion
	D2 conversion
	Read ADC result(24 bit pressure/temperature)
*/
bool MS5611::Initialize(){
	_mConvertPressure = false;
	_mHealthy = false;
	if(!_mi2c.WaitFree())
		if(!_mi2c.Initialize()) return false;
	
	u8 CMD = MS5611_RESET;
	//reset MS5611 noAddr write
	_mi2c.AddCommand(MS5611_ADDR,&CMD,1,0,0,this,true);
	if(!_mi2c.Start())		
		return false;
	if(!_mi2c.WaitFree())
		return false;
	for(volatile u32 i=0;i<500000;i++);
	
	//read Rom data of MS5611 for Calibrating
	for(u8 i=0;i<6;i++){
		bool isTaskTail = ((i==5)? true:false);
		CMD = MS5611_PROM_BASE_ADDR + i*2;
		_mi2c.AddCommand(MS5611_ADDR , &CMD , 1 , _mRomData+i*2 , 2 , this , isTaskTail);
	}
	
	if(!_mi2c.Start())		
		return false;
		IsSuccess = true;
	if(!_mi2c.WaitFree())
		return false;
	_mHealthy = true;
	
	for(u8 i=0;i<6;i++)
		_mC[i] = u16((_mRomData[i*2]<<8) | _mRomData[i*2+1]);
	
	//IsSuccess = true;
	return true;
}


bool MS5611::Update(float &pressure){

	IsSuccess = false;
	//if I2c Not work normally
	if(!_mi2c.IsHealthy()){
		_mi2c.Initialize();
		Initialize();
		return false;
	}
	static double currentTime=SysTime::Now();
	
	//Not to Updated
	if(_mIsUpdated==false){
	 //If too long in Error-> Reset
	 if(currentTime - _mUpdatedTime > 1){
				_mi2c.Initialize();
				Initialize();
				_mIsUpdated = true;
	 }
	 return false;
	}
	
		//minimal interval > 10ms
//	if(currentTime - _mUpdatedTime<0.01) 
//		return false;
	
	
	
	_mIsUpdated = false;
 
	static u8 CMD[3] = { 0   , MS5611_D1+MS5611_OSR_4096 , MS5611_D2+MS5611_OSR_4096};
//	uint8_t PReg = MS5611_D1+MS5611_OSR_4096;
//	uint8_t TReg = MS5611_D2+MS5611_OSR_4096;
	if(_mConvertPressure){
		//read pressure data
		_mi2c.AddCommand(MS5611_ADDR , CMD , 1 , _mRawT , 3 ,this , false);
		_mi2c.AddCommand(MS5611_ADDR , CMD+1,1 ,  0     , 0 ,this , true);
	}else{
		//read temperature data
		_mi2c.AddCommand(MS5611_ADDR , CMD , 1 , _mRawP , 3 ,this , false);
		_mi2c.AddCommand(MS5611_ADDR , CMD+2,1 ,  0     , 0 ,this , true);
	}
	
	//stuck in here ??
	if(!_mi2c.Start())		
		return false;
	IsSuccess = true;
	_mConvertPressure = !_mConvertPressure;
	pressure = _Baro_Origin = Pressure();
	
	
	return true;
}


float MS5611::Temperature(){
	static int64_t deltaT;
	static int32_t temperature;
	deltaT = int32_t(((u32)(_mRawT[0]<<16))|((u32)(_mRawT[1])<<8)|_mRawT[2]) - u32(_mC[4]<<8);
	temperature = 2000 + ((deltaT*_mC[5])>>23);
	if(temperature<2000)
		temperature = temperature - ((deltaT*deltaT)>>31);
	return temperature/100.0f;	
}


float MS5611::Pressure(){
	int64_t OFF2=0;
	int64_t SENS2=0;
	int64_t dT = ((u32)(_mRawT[0]<<16)|(u32)(_mRawT[1]<<8)|_mRawT[2])   -  (_mC[4]<<8 );	
	int64_t OFF =(int64_t(_mC[1])<<16) + ((int64_t(_mC[3]*dT))>>7);
	int64_t SENS=((int64_t)(_mC[0])<<15) + ( ((int64_t)(_mC[2])*dT) >>8 );
	int32_t TEMP=2000 + ((dT*(_mC[5]))>>23);
	
	//SECOND ORDER TEMPERATURE COMPENSATION
	if(TEMP<2000)
	{
		OFF2=(5*(TEMP-2000)*(TEMP-2000))>>1;
		SENS2=(5*(TEMP-2000)*(TEMP-2000))>>2;
		if(TEMP<-1500)
		{
			OFF2 = OFF2 + 7*(TEMP+1500)*(TEMP+1500);
			SENS2 = SENS2 + ((11*(TEMP+1500)*(TEMP+1500))>>1);
		}
	}
	OFF  -= OFF2;
	SENS -= SENS2;
	return (((((_mRawP[0]<<16|_mRawP[1]<<8|_mRawP[2]) *SENS) >>21) - OFF)>>15)/100.0f;	
}

float MS5611::toAltitude(float pressure){
	return 44300*(1-powf((pressure/101325),(1/5.256)));
}

