#include"Safety.h"
int Safety::safe = 1;


void Safety::Initialize(){
	safe = 0;
	
	safe = 1;
}
void Safety::updata(){
	//static int cnt = 0;
	if(_sbus._FLIGHTMODE_ == NONE_F){//����ģʽ����
		safe = 0;
		return;
	}

	if(_sbus.tagOnline == false){//ң��������
		safe = 0;
	return;
	}
	
	if(_fly_state)safe = 0;
//	if(pitch>60||roll>60||pitch<-60||roll<-60){//�ɻ��Ƕȹ���
//		safe = 0;
//		return;
//}

	safe = 1;
}

