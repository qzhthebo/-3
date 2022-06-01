#include "State.h"

//int State::state = RUNNING;
//void State::Updata(){
//	State::state = RUNNING;
//}
//void State::Initialize(){
//	State::state = RUNNING;
//}


int State::state = INIT;
void State::Initialize(){
	state = INIT;
}
void State::Updata()
 {
	 static int low_vol_cnt,low_throl_cnt;
	 switch(State::state){	 
		 case INIT:
			 if(_remote.State == REMOTECONTROL_CALIB && _sbus._FLIGHTMODE_ == STOP)state = CAL;						
			 if(_remote.State == REMOTECONTROL_UNLOCK && _sbus.MtrEn && Safety::safe && _sbus._FLIGHTMODE_!=STOP)state = STANDBY;//ready to fly
			 break;
		 
		 case CAL:
			 if(_remote.State == REMOTECONTROL_LOCK){state = INIT;_remote.State = REMOTECONTROL_LOCK;
				//b.bi();
				}
		   break;
				
		 case STANDBY:
			 if(!Safety::safe)state = ERROR;//error		
			 if(_remote.State == REMOTECONTROL_LOCK || _sbus.MtrEn==0||_sbus._FLIGHTMODE_ == STOP){state = INIT;_remote.State = REMOTECONTROL_LOCK;}
			 if(_sbus.Channel[RC_THROL] >= 10&&Safety::safe)state = RUNNING;//fly
			 if(_adc.Is_LowVol()){
				  low_vol_cnt++;//
					if(low_vol_cnt>200000){//
						 low_vol_cnt=0;
						_remote.State = REMOTECONTROL_LOCK;//lock
						}
					}
				low_throl_cnt++;//
				if(low_throl_cnt>5000){
					 low_throl_cnt=0;
					 _remote.State = REMOTECONTROL_LOCK;
					}
			break;	

		 case RUNNING:
			 	if(!Safety::safe)state = ERROR;//error
				if(_sbus.Channel[RC_THROL]<=10)state = STANDBY;//standby
		    break;
		 
		 default:
			  state = INIT;
			  break;
		}
	}
     
	       
	
