#include "HMC5883l.h"



HMC5883L::HMC5883L(I2C &i2c):_i2c(i2c){
	is_Success= false;
	Initialize();
}

bool HMC5883L::Initialize(){
	_mHealthy = false;
	if(!_i2c.WaitFree())
		if(!_i2c.Initialize())  return false;
	
	u8 ConfigData[10][2]={
		HMC5883L_ConfigurationRegisterA,0x78,  //配置寄存器A：采样平均数1 输出速率30Hz 正常测量
		HMC5883L_ConfigurationRegisterB,0x20,   //配置寄存器B：增益控制
		HMC5883L_ModeRegister,0x00              //模式寄存器：连续测量模式
	};
	
	for(u8 i=0;i<3;i++){
		bool isTaskTail = ((i==2) ? true :false);
		_i2c.AddCommand(HMC5883L_Addr,ConfigData[i],2,0,0,this,isTaskTail);
	}
	if(!_i2c.Start()) return false;
	
	//wait initialize complete, 
	if(!_i2c.WaitFree()) return false;
	is_Success = true;
	
	_mHealthy = true;	
	return true;
}

u8 HMC5883L::Update_data(Vector3f &magnet){
	//If I2C Bus Error
	if(!_i2c.IsHealthy()){
		_i2c.Initialize();
		Initialize();
		return 1;
	}
	
	
	 //Not to Updated
	if(_mIsUpdated==false){
	 //If too long in Error-> Reset
	 double currentTime=SysTime::Now();
	 if(currentTime - _mUpdatedTime > 0.05){
				_i2c.Initialize();
				Initialize();
				_mIsUpdated = true;
	 }
	 return 2;
 }
	
	 //Read data
	 _mIsUpdated=false;
	 uint8_t Reg_data = HMC5883L_Output_X_MSB;
	 _i2c.AddCommand(HMC5883L_Addr,&Reg_data,1,_Sensor_data,6,this,true);
	 _i2c.Start();
	
	_Mag_Origin.x = s16(_Sensor_data[0]<<8 | _Sensor_data[1]);
	_Mag_Origin.y = s16(_Sensor_data[2]<<8 | _Sensor_data[3]);
	_Mag_Origin.z = s16(_Sensor_data[4]<<8 | _Sensor_data[5]); 
 
	magnet.x = s16(_Sensor_data[0]<<8 | _Sensor_data[1]);
	magnet.y = s16(_Sensor_data[2]<<8 | _Sensor_data[3]);
	magnet.z = s16(_Sensor_data[4]<<8 | _Sensor_data[5]);  
	
	return 3;	
}
