#include "I2C.h"


I2C*	I2C::pI2C[2] = { 0 , 0 };

//	=======================================================================
//	|  I2C | remap=false |  remap=true |       IRQ   |      RCC           |
//	|------|-------------|-------------|-------------|--------------------|
//	|      |SCL | PB6    | SCL | PB8   |I2C1_EV_IRQn |                    |
//	| I2C1 |-------------|-------------|             |RCC_APB1Periph_I2C1 |
//	|      |SDA | PB7    | SDA | PB9   |I2C1_ER_IRQn |                    |
//	|------|-------------|-------------|-------------|--------------------|
//	|      |SCL | PB10   |     xxx     |I2C2_EV_IRQn |                    |
//	| I2C2 |-------------|-------------|             |RCC_APB1Periph_I2C1 |
//	|      |SDA | PB11   |     xxx     |I2C2_ER_IRQn |                    |
//	=======================================================================

I2C::I2C(I2C_TypeDef* I2Cx,u32 Speed,u8 remap,u8 priGroup,u8 preEvent,u8 subEvent,u8 preError,
u8 subError){
	//Save I2C Bus Params
	_I2Cx = I2Cx;
	_I2CSpeed = Speed;
	_remap = remap;
	_EventPre = preEvent;
	_EventSub = subEvent;
	_ERRORPre = preError;
	_ERRORSub = subError;
	_I2CState = I2C_STATE_NULL;			//I2C not Initialized
	_ERRORCount = -1;								//I2C no error
	//save GPIO Pin 
	if(_I2Cx == I2C1){
		if(_remap){
			//remap
			_SCLPin = GPIO_Pin_8;
			_SDAPin = GPIO_Pin_9;
		}else{
			//no remap
			_SCLPin = GPIO_Pin_6;
			_SDAPin = GPIO_Pin_7;
		}
		_EventIRQ = I2C1_EV_IRQn;
		_ERRORIRQ = I2C1_ER_IRQn;
		_I2CxRcc  = RCC_APB1Periph_I2C1;
		pI2C[0] = this;
	}else if(_I2Cx==I2C2){
		_SCLPin = GPIO_Pin_10;
		_SDAPin = GPIO_Pin_11;
		_EventIRQ = I2C2_EV_IRQn;
		_ERRORIRQ = I2C2_ER_IRQn;		
		_I2CxRcc  = RCC_APB1Periph_I2C2;	
		pI2C[1] = this;
	}
	// save Priority Group
	switch(priGroup){
		case 0: _PriGroup = NVIC_PriorityGroup_0; break;  
		case 1: _PriGroup = NVIC_PriorityGroup_1; break;  
		case 2: _PriGroup = NVIC_PriorityGroup_2; break;  
		case 3: _PriGroup = NVIC_PriorityGroup_3; break;  
		case 4: _PriGroup = NVIC_PriorityGroup_4; break;  
		default: _PriGroup = NVIC_PriorityGroup_3; break;  
	}
	//Initialize GPIO,NVIC,I2C...
	Initialize();
}


//===================================================  I2C Initialize
bool I2C::Initialize(){
	_ERRORCount++;  //Error Count +1
	if(!Reset()){
		//Check Reset State
		_I2CState = I2C_STATE_ERROR;
		return false;
	}
	
	InitGPIO(GPIO_Mode_AF_OD);
	InitI2C();
	InitNVIC();
	_CMDList.flush();							//Clear I2C Command List
	_I2CState = I2C_STATE_FREE;   //I2C Bus is ready
	return true;
}


//===================================================  Reset I2C Bus 
bool I2C::Reset(){
	
	//Reset I2C device and Rcc 		--   I2C->CR1 state Register 1
	_I2Cx->CR1 |=  I2C_CR1_SWRST;			//Start reset I2c  -- soft reset
	_I2Cx->CR1 &=  ~I2C_CR1_SWRST;		//Stop  reset I2C
	_I2Cx->CR1 &=  ~I2C_CR1_PE;				//DISABLE I2C
	RCC->APB1RSTR |= _I2CxRcc;				//Start reset I2C Clock
	RCC->APB1RSTR &= ~_I2CxRcc;				//Stop  reset I2C Clock
	RCC->APB1ENR |=  _I2CxRcc;				//Enable I2C clock
	
	
	//Check and solve I2C Bus Busy problem
	u8 timeoutCount=0;
	while(I2C_GetFlagStatus(_I2Cx,I2C_FLAG_BUSY) && timeoutCount<20){
		//Check Busy state and time out FLAG
		RCC_APB1PeriphClockCmd(_I2CxRcc,DISABLE);		//Close I2C Device;
		InitGPIO(GPIO_Mode_Out_PP);									//Initialize GPIO for Software handle
		GPIO_ResetBits(GPIOB,_SCLPin);
		GPIO_ResetBits(GPIOB,_SDAPin);
		for(volatile u16 i=0;i<100;i++);						// Stop Signal by Pin simulation
		GPIO_SetBits(GPIOB, _SCLPin);
		for(volatile u16 i=0;i<100;i++); 
		GPIO_SetBits(GPIOB, _SDAPin);
		for(volatile u16 i=0;i<100;i++);
		InitGPIO(GPIO_Mode_IN_FLOATING);						//Initialize GPIO for I2C Busy detect
		RCC->APB1RSTR |= _I2CxRcc;									//Start reset I2C Clock
		RCC->APB1RSTR &= ~_I2CxRcc;									//Stop  reset I2C Clock
		RCC->APB1ENR |=  _I2CxRcc;									//Enable I2C clock
		++timeoutCount;															//Time out count, maximum 20 trys
	}
	
	//Check if Reset Succeed
	if(timeoutCount>=20) return false;	//Reset failed
	return true;												//Reset succeed
}



//===================================================   Initialize I2C GPIO
void I2C::InitGPIO(GPIOMode_TypeDef GpioMode){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	if(_I2Cx==I2C1 && _remap && GpioMode == GPIO_Mode_AF_OD){
		RCC_APB2PeriphClockCmd((RCC_APB2Periph_AFIO),ENABLE);			//Enable GPIO Alternative Functions
		GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);						//Enable I2C1 pin Remap
	}
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         
	GPIO_InitStructure.GPIO_Mode = GpioMode;                  
	GPIO_InitStructure.GPIO_Pin = _SCLPin | _SDAPin;         
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//Initialize I2Cx Pin	
}



//===================================================      Initialize I2C
void I2C::InitI2C(){
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_ClockSpeed          = _I2CSpeed;                     
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;                 
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;               
	I2C_InitStructure.I2C_OwnAddress1         = 0;                            
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;             
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  
	I2C_Init(_I2Cx,&I2C_InitStructure);							
}	



//===================================================      Initialize NVIC
void I2C::InitNVIC(){
	//I2C Event NVIC
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 
	NVIC_InitStructure.NVIC_IRQChannel = _EventIRQ;                  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _EventPre; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _EventSub;   
	NVIC_Init(&NVIC_InitStructure);	

	//I2C Error NVIC
  	NVIC_InitStructure.NVIC_IRQChannel = _ERRORIRQ;                   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _ERRORPre; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _ERRORSub;        
  	NVIC_Init(&NVIC_InitStructure);	

	//Enable I2C IRQ AND Global Priority Group
	//Enable Event , Error , Buffer IRQ
	_I2Cx->CR2 |= (I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_IT_BUF );
	NVIC_PriorityGroupConfig(_PriGroup);
}	



//===================================================     ADD COMMAND to buffer
bool I2C::AddCommand(u8 slaveAddr,u8 txData[],u8 txNumber,u8 rxData[],u8 rxNumber
,Sensor *pDevice, bool isTaskTail){
	if(_CMDList.size() >= I2C_QUEUE_SIZE)
		return false;
	
	I2C_Command i2cCMD;
	i2cCMD.slaveAddr = slaveAddr;
	i2cCMD.outDataLen = txNumber;
	i2cCMD.inDataLen = rxNumber;
	i2cCMD.pDataIn = rxData;
	i2cCMD.pDevice = pDevice;
	i2cCMD.isTaskTail = isTaskTail;
	for(u8 i=0;i<txNumber;i++)
		i2cCMD.dataOut[i] = txData[i];
	_CMDList.PutData(i2cCMD);
	return true;
}
	

//===================================================    Execute Next Command
bool I2C::StartNextCMD(){
	if(_I2CState==I2C_STATE_FREE || _I2CState==I2C_STATE_STOP){
		//Check I2C is free or generate a Stop signal
		if(_CMDList.GetData(_CurrentCMD)){
			_I2CState = I2C_STATE_START;		//generate start signal
			_Dataindex = 0;									//reset data index
			if(_CurrentCMD.outDataLen>0)		//send mode
				_I2CxDirection = I2C_Direction_Transmitter;
			else														//receive mode
				_I2CxDirection = I2C_Direction_Receiver;
			I2C_AcknowledgeConfig(_I2Cx,ENABLE);			 //Enable Ack 
			I2C_GenerateSTART(_I2Cx,ENABLE);					 //Generate a Start signal, will interrupt later
			return true;
		}
		_I2CState = I2C_STATE_FREE;       //Not Commamd		
	}
	
	return false;												//Start a I2C Command failed
}



//===================================================     Start to Execute I2C Command
bool I2C::Start(){
	if(_I2CState == I2C_STATE_FREE)		//Check I2C Bus status
		return StartNextCMD();					//Start to Execute I2C Command
	return false;
}	


//===================================================     Get state of I2C Bus
bool I2C::IsHealthy(){
	if(_I2CState==I2C_STATE_ERROR)
		return false;
	return true;
}	


//===================================================			Check I2C Bus is free or not
bool I2C::IsFree(){
	if(_I2CState==I2C_STATE_FREE) //Check I2C's free flag
		return true;								//I2C is free
	return false;									//I2C is busy
}	


//===================================================			wait untile I2C Bus is free
bool I2C::WaitFree(){
	//true-> I2C is free
	//false-> I2C is still busy or timeout
	volatile u32 timeoutcount=0;
	//Wait untile I2C bus free or reach maximum time out count
	while(_I2CState!=I2C_STATE_FREE && ++timeoutcount<500000);
	if(_I2CState == I2C_STATE_FREE)
		return true;				//I2C bus is free
	return false;					//I2C bus is still busy or timeout
}	






//===================================================			Interrupt Function

void I2C::EventIRQ(){
	//Avoid Dead Loop
	static u8 ResetCount = 0;						
	if(++ResetCount>100){
		_I2CState = I2C_STATE_ERROR;		//I2C Error
		_I2Cx->CR1 |= I2C_CR1_SWRST;		//I2C start reset 
		_I2Cx->CR1 &= ~I2C_CR1_SWRST;		//I2C end reset
		ResetCount = 0;
	}
	
	//Event IRQ Handler
	switch(I2C_GetLastEvent(_I2Cx)){
		//check I2C Event IRQ flag
		case I2C_EVENT_MASTER_MODE_SELECT:									//EV5:Start signal finished
				_I2CState = I2C_STATE_ADDR;				//next to send addr
				_Dataindex = 0;										//Reset data index 
				I2C_Send7bitAddress(_I2Cx,_CurrentCMD.slaveAddr,_I2CxDirection);	
				//Send slave adress to activate slave
				break;
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:		//EV6:Slave adress send complete/Tx
				_I2CState = I2C_STATE_TXMOD;
				I2C_SendData(_I2Cx,_CurrentCMD.dataOut[_Dataindex++]);	//Send a byte to slave
				break;
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:				//EV6:Slave adress send complete/Rx
				_I2CState = I2C_STATE_RXMOD;
				if(_CurrentCMD.inDataLen<=1){												//Only one byte need to receive
					I2C_AcknowledgeConfig(_I2Cx,DISABLE);							//No Need to send a Ack
					I2C_GenerateSTOP(_I2Cx,ENABLE);										//Ready to generate Stop signal
				}
				break;
		case I2C_EVENT_MASTER_BYTE_RECEIVED:								//EV7: one byte received / RX
				_I2CState = I2C_STATE_RX;
				_CurrentCMD.pDataIn[_Dataindex++] = I2C_ReceiveData(_I2Cx); //Save one byte data
				if(_CurrentCMD.inDataLen - _Dataindex ==1){
					I2C_AcknowledgeConfig(_I2Cx,DISABLE);							//DISABLE to ack
					I2C_GenerateSTOP(_I2Cx,ENABLE);										//Ready to generate stop signal
				}else if(_CurrentCMD.inDataLen == _Dataindex){			//All data are received
					_I2CState = I2C_STATE_STOP;
					ResetCount=0;																			//I2C communication success
					if(_CurrentCMD.isTaskTail &&_CurrentCMD.pDevice)	//if current command is the end of command sequnece
						_CurrentCMD.pDevice->Updated();									//Update finished time stamp
					StartNextCMD();																		//Execute next I2C command
				}
				break;
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED:							//EV8: one byte send complete/Tx
				if(_Dataindex < _CurrentCMD.outDataLen){						//still have data need to send
					_I2CState = I2C_STATE_TX;													//Set to Tx mode
					I2C_SendData(_I2Cx,_CurrentCMD.dataOut[_Dataindex++]);	//send one byte data
				}else{																							//All data send complete
					if(_CurrentCMD.inDataLen>0){											//if need to receive data
						_I2CState = I2C_STATE_START;										//Set to Start state
						_Dataindex = 0;
						_I2CxDirection = I2C_Direction_Receiver;        //change direction
						I2C_GenerateSTART(_I2Cx,ENABLE);								//Generate start signal
					}else{																						//no need to receive data
						_I2CState = I2C_STATE_STOP;											//Set to Stop state
						I2C_GenerateSTOP(_I2Cx,ENABLE);									//Generate Stop signal
						ResetCount=0;																		//I2C communication success
						if(_CurrentCMD.isTaskTail&& _CurrentCMD.pDevice)//if current command is the end of command sequnece
							_CurrentCMD.pDevice->Updated();
						StartNextCMD();																	//Execute next I2C command
					}
				}
				break;
	}	
}
void I2C::ERRORIRQ(){
	//Reset I2C to avoid Error
	_I2Cx->CR1 |= I2C_CR1_SWRST; 	//Start reset I2C
	_I2Cx->CR1 &= ~I2C_CR1_SWRST; //End reset I2C
	_I2CState = I2C_STATE_ERROR;  //Set to error state
}


