#include "UART.h"    		
#include "SysTime.h" 		
#include "UIDLink.h"
#include "led.h"

/*
创建于2022=5-31
作者：11903990524-秦侦洪
*/
#define COM_PID_PARAMS				0x01//功能码
#define COM_FLY_DATA					0x02

//功能码   传送长度，接收长度
u8 Protocol[10][3]={
										COM_PID_PARAMS  ,   1   ,		1	,
										COM_FLY_DATA		,		53	 ,		1		,
					};

					
	




UART com1(1,115200);   //定义串口号，波特率

UIDLink _UIDLink(com1,Protocol,5,0xAA,0xAF);	//定义传输帧结构


void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置, 推挽输出
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 
}

int main(){	
	LED_Init();
	SysTime::Initialize();//定时器初始化
	u8 count = 0,cnt = 0,count2=0;
	u8 h_100Hz = SysTime::CreateTimer(0.1);//定义定时器
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


