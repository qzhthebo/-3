#include "MPU6050.h"
#include "GPIO.h"

//Constructor
MPU6050::MPU6050(I2C &i2c,bool _Zero_Calibrate,bool _EN_LPF,bool _EN_Move):_i2c(i2c){
	
	Is_Success = false;
	if(_Zero_Calibrate) 
		StartGyroCalibrating();
	Initialize_IMU();
	EN_Filter_LPF = _EN_LPF;
	EN_Filter_AntiPulseMoveAverage = _EN_Move;
}

//Initialize Mpu6050
//refer to MPU6050 datasheet
bool MPU6050::Initialize_IMU(){
	 _mHealthy = false;
		if(!_i2c.WaitFree())
			if(!_i2c.Initialize()) return false;
	
		//Config Mpu
		u8 ConfigData[10][2] ={
			MPU_PWR_MGMT1_REG , 0x03,  
			MPU_GYRO_CFG_REG  , 0x18,		//±2000 °/s
			MPU_ACCEL_CFG_REG , 0x00,
			MPU_CFG_REG				, 0x00,
			MPU_SAMPLE_RATE_REG,0x00,
			
			//Enable bypass mode
			MPU_USER_CTRL_REG , 0x00,
			MPU_INTBP_CFG_REG , 0x02
		};
		
		//Add I2c command
		for(u8 i=0;i<7;i++){
			bool isTaskTail = ((i==6)? true: false);
			_i2c.AddCommand(MPU_IIC_ADDR,ConfigData[i],2,0,0,this,isTaskTail);
		}
		//Start to run
		_i2c.Start();
	
		//wait mpu6050 initialize complete, 
		if(!_i2c.WaitFree()) return false;             
		_mHealthy = true;
		Is_Success = true;
		return true;
}


bool MPU6050::Update_data(Vector3f &Acc,Vector3f &Gyro){
	//If I2C Bus Error
	 if(!_i2c.IsHealthy()){
		 _i2c.Initialize();
		 Initialize_IMU();
		 return false;
	 }
	 
	 //Not to Updated
	 if(_mIsUpdated==false){
		 //If too long in Error-> Reset
		 double currentTime=SysTime::Now();
		 if(currentTime - _mUpdatedTime > 0.05){
			 		_i2c.Initialize();
					Initialize_IMU();
					_mIsUpdated = true;
		 }
		 return false;
	 }
	 
	 //Read data
	 _mIsUpdated=false;
	 uint8_t Reg_data = MPU_ACCEL_XOUTH_REG;
	 _i2c.AddCommand(MPU_IIC_ADDR,&Reg_data,1,_Sensor_data,14,this,true);
	 _i2c.Start();
	 
	 
	 //======================================================Gyro Zero Offset Measurement
	 if(IstoCalibrate()){
		 _Gyro_Calibrate_Sum.x += _Gyro_Origin.x;
		 _Gyro_Calibrate_Sum.y += _Gyro_Origin.y;
		 _Gyro_Calibrate_Sum.z += _Gyro_Origin.z;
		 if(++_Gyro_Calibrate_count>=OFFSET_AV_NUM){
			 StopCalibrating();
			 _Gyro_Zero_Offset.x = _Gyro_Calibrate_Sum.x / OFFSET_AV_NUM;
			 _Gyro_Zero_Offset.y = _Gyro_Calibrate_Sum.y / OFFSET_AV_NUM;
			 _Gyro_Zero_Offset.z = _Gyro_Calibrate_Sum.z / OFFSET_AV_NUM;
		 }
	 }
	 
	_Acc_Origin.x = s16(_Sensor_data[0]<<8 | _Sensor_data[1]);
	_Acc_Origin.y = s16(_Sensor_data[2]<<8 | _Sensor_data[3]);
	_Acc_Origin.z = s16(_Sensor_data[4]<<8 | _Sensor_data[5]);
	_Gyro_Origin.x = s16(_Sensor_data[8]<<8 | _Sensor_data[9]);
	_Gyro_Origin.y = s16(_Sensor_data[10]<<8 | _Sensor_data[11]);
	_Gyro_Origin.z = s16(_Sensor_data[12]<<8 | _Sensor_data[13]);
	 
	_Gyro_Origin -= _Gyro_Zero_Offset;
	 
	 
	 //======================================================Acc LPF Filter
	 if(EN_Filter_LPF){
			_Acc_Origin.x = Acc_LPF[0].Get(_Acc_Origin.x);
			_Acc_Origin.y = Acc_LPF[1].Get(_Acc_Origin.y);
			_Acc_Origin.z = Acc_LPF[2].Get(_Acc_Origin.z);			 
	 }
	 
	 //======================================================Acc MoveAverage Filter
	 if(EN_Filter_AntiPulseMoveAverage){
			_Acc_Origin.x = Acc_Mov[0].GetAntiPulse(_Acc_Origin.x);
			_Acc_Origin.y = Acc_Mov[1].GetAntiPulse(_Acc_Origin.y);
			_Acc_Origin.z = Acc_Mov[2].GetAntiPulse(_Acc_Origin.z);
	 }
	 	 
	_Acc_G.x = _Acc_Origin.x / ACC_2G_SCALE;
	_Acc_G.y = _Acc_Origin.y / ACC_2G_SCALE;
	_Acc_G.z = _Acc_Origin.z / ACC_2G_SCALE;
	 
	_Gyro.x = _Gyro_Origin.x * (1.0/(65536.0/2.0))*2000.0;
	_Gyro.y = _Gyro_Origin.y * (1.0/(65536.0/2.0))*2000.0;
	_Gyro.z = _Gyro_Origin.z * (1.0/(65536.0/2.0))*2000.0;
	 
	 
	 //Acc(_Acc_G.x,_Acc_G.y,_Acc_G.z);
	 Acc(_Acc_Origin.x,_Acc_Origin.y,_Acc_Origin.z);
	 //Gyro(_Gyro_Origin.x,_Gyro_Origin.y,_Gyro_Origin.z);
	 Gyro(_Gyro.x,_Gyro.y,_Gyro.z);
	 //Gyro = _Gyro_Origin;
	 
	 return true;
}


	 

