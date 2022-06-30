#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "Sensor.h"

//the size of I2C Command Buffer
#define I2C_QUEUE_SIZE 20

//Constants to identify I2C bus's current status
#define I2C_STATE_NULL 	0x00	//Not working,not Initialized
#define I2C_STATE_START 0x01    //generate a start signal
#define I2C_STATE_ADDR  0x02    //Send a Adress Byte to Slave
#define I2C_STATE_TXMOD 0x03    //After Send Addr, on Transmit Mode
#define I2C_STATE_RXMOD 0x04    //After Send Addr, on Received Mode
#define I2C_STATE_TX    0x05    //Sending a Byte to Slave
#define I2C_STATE_RX    0x06    //Receiving a Byte From Master
#define I2C_STATE_STOP  0x07    //generate a stop signal
#define I2C_STATE_FREE  0x08    //current I2C Bus is free
#define I2C_STATE_ERROR 0x09    //I2C bus ERROR

//I2C Command Struct
struct I2C_Command
{
	u8 slaveAddr;		//slave address
	u8 dataOut[5];		//bytes send to slave,max 5 bytes one time
	u8 outDataLen;  	//number of bytes send to slave
	u8* pDataIn;    	//receive bytes from slave
	u8 inDataLen;   	//number of receive bytes
	u8 isTaskTail;  	//Current I2C command is the last or not
	Sensor *pDevice;	//Mechine Status
};

class I2C{
private:
	//params for setting I2C Bus
	I2C_TypeDef* 	_I2Cx;  		//I2C Bus Selected, I2C1 or I2C2
	u32 		 			_I2CSpeed;		//I2C Bus Speed: 100k 200k 400k
	bool         	_remap;		
	u16          	_SDAPin;
	u16          	_SCLPin;
	u32 		 			_I2CxRcc;		//I2C Clock

	//params for I2C Interrupt
	u32 		 _PriGroup;		//Priority Group
	u8			 _EventIRQ;		//I2C Event IRQn
	u8			 _ERRORIRQ;     //I2C ERROR IRQn
	u8 			 _EventPre;     //Event Preemption Priority
	u8			 _EventSub;		//Event Sub Priority
	u8			 _ERRORPre;		//ERROR Preemption Priority
	u8			 _ERRORSub;		//ERROR Sub Priority

	//params for I2C Bus Runtime
	u8			 																_I2CState;								//I2C Bus State Flag
	FIFOBuffer<I2C_Command,I2C_QUEUE_SIZE>  _CMDList;	//Command List
	u8           														_Dataindex;							//index of Tx or Rx data
	u8			 																_I2CxDirection;						//I2C Bus Direction
	I2C_Command	 														_CurrentCMD;							//Current I2C Command
	int 		 																_ERRORCount;							//I2C Bus ERROR count

public:
	static I2C* pI2C[2];

private:
	//Inner Called
	bool Reset();									//Reset I2C Bus
	void InitGPIO(GPIOMode_TypeDef GpioMode);	//Initialize I2C GPIO
	void InitI2C();								//Initialize I2C
	void InitNVIC();							//Initialize NVIC
	bool StartNextCMD();						//Execute Next Command
public:
	//User Called
	I2C(I2C_TypeDef* I2Cx,u32 Speed=400000,u8 remap=0,u8 priGroup=3,u8 preEvent=0,u8 subEvent=0,u8 preError=0,u8 subError=0);
	bool Initialize();							//Initialize I2C Bus
	bool AddCommand(u8 slaveAddr,u8 txData[],u8 txNumber,u8 rxData[],u8 rxNumber,Sensor *pDevice, bool isTaskTail);
												//Add a Command 
	bool Start();								//Start to Execute I2C Command
	bool IsHealthy();							//Get state of I2C Bus
	bool IsFree();								//Check I2C Bus is free or not
	bool WaitFree();							//wait untile I2C Bus is free
	int I2CErrors(){
		return _ERRORCount;						//Get the ERROR STATE
	}
	u8 I2C_State(){return _I2CState;}
public:
	u8 CMDNumber(){return _CMDList._size_;}
	//Interrupt Function
	void EventIRQ();
	void ERRORIRQ();
};







#endif
