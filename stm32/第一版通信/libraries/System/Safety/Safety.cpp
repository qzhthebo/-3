#include"Safety.h"
int Safety::safe = 1;


void Safety::Initialize(){
	safe = 0;
	
	safe = 1;
}
void Safety::updata(){
	//static int cnt = 0;
	if(_sbus._FLIGHTMODE_ == NONE_F){//飞行模式错误
		safe = 0;
		return;
	}

	if(_sbus.tagOnline == false){//遥控器掉线
		safe = 0;
	return;
	}
	
	if(_fly_state)safe = 0;
//	if(pitch>60||roll>60||pitch<-60||roll<-60){//飞机角度过大
//		safe = 0;
//		return;
//}

	safe = 1;
}

