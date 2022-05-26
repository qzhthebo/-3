#include "buzzer.h"

buzzer::buzzer(GPIO &_p):p(_p){
}
void buzzer::bi(){
	p=1;
	for(int i=10000;i>0;i--)
	p=0;
}
void buzzer::bi(int n){
	p=1;
	for(int i=10000;i>0;i--)
	p=0;
	
}
