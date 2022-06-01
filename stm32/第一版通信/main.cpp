#include "UART.h"    		
#include "SysTime.h" 		
#include "UIDLink.h"

#define COM_PID_PARAMS				0x01//������
#define COM_FLY_DATA					0x02

//������   ���ͳ��ȣ����ճ���
u8 Protocol[10][3]={
										COM_PID_PARAMS  ,   1   ,		1	,
										COM_FLY_DATA		,		53	 ,		1		,
					};

					
	




UART com1(1,115200);   //���崮�ںţ�������

UIDLink _UIDLink(com1,Protocol,5,0xAA,0xAF);	//���崫��֡�ṹ



int main(){	
	SysTime::Initialize();//��ʱ����ʼ��
	u8 count = 0,cnt = 0,count2=0;
	u8 h_1000Hz = SysTime::CreateTimer(0.1);//���嶨ʱ��
	com1.Precision(1);
	uint16_t n=0;


while(1){		
			if(SysTime::CheckTimer(h_1000Hz)){
      count++;		
      if(count>=1000){
				count=1;
			}
			cnt++;
			if(cnt>=3)
			{
				cnt=0;
			}
			if(cnt==1){
				u8 t=1;
			_UIDLink._Tx_Frame._FunctionCode = COM_PID_PARAMS;
			_UIDLink.FillUnSigned8(0,t);
			_UIDLink.Pack_Send();
			}
			
		}
	}

}


