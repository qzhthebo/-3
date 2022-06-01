#include "SBUS.h"



SBUS::SBUS(UART &com):_com(com),_Rx_Buffer(com._RX_Buffer){
	tagOnline = false;			//Flag for Remotor online
	cntUpdate = 1;     //Counter for Update Interval
	MtrEn = 0;//电机使能
}





void SBUS::Update(){
	static int maxCNT = 10;
	
	++cntUpdate;
	if(Raw[23]!=0){
		cntUpdate = maxCNT+10;
		tagOnline = false;
		return;
		//掉线检测
//		_ROLL_ = 0;
//		_PITCH_= 0;
//		_THROL_= 0;
//		_YAW_  = 0;
//		_FLIGHTMODE_ = NONE_F;
//		_SIGHTMODE_	 = NONE_S;
//		_UPSIDE_WAVE_=	0;
//		_UPSIDE_LEAN_=	0;
	}
	//Find and Check Header = 0x0F
	while(_Rx_Buffer._size_>0&&(_Rx_Buffer._buffer[_Rx_Buffer._index_out]!=0x0F)){
		if(++_Rx_Buffer._index_out >= _Rx_Buffer._max_size_)
			_Rx_Buffer._index_out = 0;
		_Rx_Buffer._size_--;
	}
	
	//Check Data Length = 25
	if(_Rx_Buffer._size_<25)
		return;
	
	//Check End Code = 0x00
	u16 index = _Rx_Buffer._index_out + 24;
	if(index >= _Rx_Buffer._max_size_)
		index -= _Rx_Buffer._max_size_;
	
	//Get End Code
	u8 FnCode = _Rx_Buffer._buffer[index];
	
	//End Code Error
	if(FnCode!=0x00){
		_Rx_Buffer.GetData(FnCode);
		return;
	}
	
	//Read one dataFrame of SBUS
	_Rx_Buffer.GetDatas(Raw,25);
	
	//Update Channel_Origin data
	Channel_Origin[0] = Raw[1] | ((Raw[2] & 0x07)<<8);                      		//Ch1 = 8 + 3
	Channel_Origin[1] = (Raw[2]>>3) | ((Raw[3] & 0x3F)<<5);                 		//Ch2 = 5 + 6
	Channel_Origin[2] = (Raw[3]>>6) | (Raw[4]<<2) | ((Raw[5] & 0x01)<<10);  		//Ch3 = 2 + 8 + 1
	Channel_Origin[3] = (Raw[5]>>1) | ((Raw[6] & 0x0F)<<7);                 		//Ch4 = 7 + 4
	Channel_Origin[4] = (Raw[6]>>4) | ((Raw[7] & 0x7F)<<4);                 		//Ch5 = 4 + 7
	Channel_Origin[5] = (Raw[7]>>7) | (Raw[8]<<1) | ((Raw[9] & 0x03)<<9);   		//Ch6 = 1 + 8 + 2
	Channel_Origin[6] = (Raw[9]>>2) | ((Raw[10] & 0x1F)<<6);										//Ch7 = 6 + 5
	Channel_Origin[7] = (Raw[10]>>5) | (Raw[11]<<3);                           //Ch8 = 3 + 8
	Channel_Origin[8] = Raw[12] | ((Raw[13] & 0x07)<<8);                       //Ch9 = 8 + 3
	Channel_Origin[9] = (Raw[13]>>3) | ((Raw[14] & 0x3F)<<5);                  //Ch10= 5 + 6
	Channel_Origin[10]= (Raw[14]>>6) | (Raw[15]<<2) | ((Raw[16] & 0x01)<<10);  //Ch11= 2 + 8 + 1
	Channel_Origin[11]= (Raw[16]>>1) | ((Raw[17] & 0x0F)<<7);                  //Ch12= 7 + 4
	Channel_Origin[12]= (Raw[17]>>4) | ((Raw[18] & 0x7F)<<4);                  //Ch13= 4 + 7
  Channel_Origin[13]= (Raw[18]>>7) | (Raw[19]<<1) | ((Raw[20] & 0x03)<<9);   //Ch14= 1 + 8 + 2
 	Channel_Origin[14]= (Raw[20]>>2) | ((Raw[21] & 0x1F)<<6);                  //Ch15= 6 + 5
 	Channel_Origin[15]= (Raw[21]>>5) | (Raw[22]<<3);                           //Ch16= 3 + 8		
	
	//Turn Origin Value to Percent (there are Flight Control)
	for(u8 i=0;i<16;i++){
		if(Channel_Origin[i]<=SBUS_MIN)
			Channel_Origin[i]=SBUS_MIN;
		else if(Channel_Origin[i]>=SBUS_MAX)
			Channel_Origin[i] = SBUS_MAX;
		if(i == FLIGHTMODE || i == SIGHTMODE)
			continue;
		//Turn to 0~100
		Channel[i] = (Channel_Origin[i]-SBUS_MIN)/13.87f;
	}
	
	_ROLL_ = Channel[RC_ROLL];
	_PITCH_ = Channel[RC_PITCH];
	_YAW_  =  Channel[RC_YAW];
	_THROL_ = Channel[RC_THROL];
	
	switch(Channel_Origin[FLIGHTMODE]){
		case 306:
			if(Channel[RC_THROL] <= 2) 
			_FLIGHTMODE_ = NORMAL;
			break;
		case 1000:
			if(Channel[RC_THROL] <= 2)
			_FLIGHTMODE_ = DEBUG;
			break;
		case 1693:
			if(Channel[RC_THROL] <= 2)
			_FLIGHTMODE_ = STOP;
			break;
		default:
			_FLIGHTMODE_ = NONE_F;
			break;
	}
	
	switch(Channel_Origin[SIGHTMODE]){
		case 306:
			_SIGHTMODE_ = VIDEO;
			break;
		case 1000:
			_SIGHTMODE_ = STOP1;
			break;
		case 1694:
			_SIGHTMODE_ = PHOTO;
			break;
		default:
			_SIGHTMODE_ = NONE_S;
			break;
	}

	
	
	switch(Channel_Origin[6]){
		case 306:		
				MtrEn = 0;
				break;
		case 1693:
			if(_THROL_ == 0)
				MtrEn = 1;
				break;
	}
	
	if(Channel_Origin[6] == 306)MtrEn=0;
	if(Channel_Origin[6] == 1693 && _THROL_ == 0)MtrEn = 1;
	
	_UPSIDE_WAVE_ = Channel[UPSIDEWAVE];
	_UPSIDE_LEAN_ = Channel[UPSIDELEAN];
	
	if(cntUpdate == 2 || cntUpdate >=10)
		RemotorUpdateFreq = 50;
	else
		RemotorUpdateFreq = 1000/((cntUpdate-1)*20);
	
	tagOnline = true;
	cntUpdate = 1;
	
	if(_Rx_Buffer._size_>=25)
		Update();
}

