#include "UART.h"    		
#include "SysTime.h" 		
#include "UIDLink.h"
#include "led.h"

/*
������2022=5-31
���ߣ�11903990524-�����
*/
#define COM_PID_PARAMS				0x01//������
#define COM_FLY_DATA					0x02

//������   ���ͳ��ȣ����ճ���
u8 Protocol[10][3]={
										COM_PID_PARAMS  ,   1   ,		1	,
										COM_FLY_DATA		,		53	 ,		1		,
					};

					
	




UART com1(1,115200);   //���崮�ںţ�������

UIDLink _UIDLink(com1,Protocol,5,0xAA,0xAF);	//���崫��֡�ṹ


void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 
}

int main(){	
	LED_Init();
	SysTime::Initialize();//��ʱ����ʼ��
	u8 count = 0,cnt = 0,count2=0;
	u8 h_100Hz = SysTime::CreateTimer(0.1);//���嶨ʱ��
	com1.Precision(1);
	uint16_t n=0;
	LED0=1;
	LED1=1;
	u8 keyled1=0;
	u8 keyled2=0;


while(1){		
			if(SysTime::CheckTimer(h_100Hz)){
      count++;		
      if(count>=3){
				count=1;
			}
			if(_UIDLink.Pack_Check()==COM_PID_PARAMS){
			keyled1=1-keyled1;
			_UIDLink._Tx_Frame._FunctionCode = COM_PID_PARAMS;
			_UIDLink.FillUnSigned8(0,_UIDLink.GetUnSigned8(0));
			_UIDLink.Pack_Send();		
			}
			if(keyled1 && count==1)
			{
				LED0=1-LED0;
			}
		}
	}

}


