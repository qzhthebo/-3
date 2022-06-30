#include "UART.h"    		
#include "SysTime.h" 		
#include "UIDLink.h"
#include "led.h"

#define COM_PID_PARAMS				0x01//功能码
#define COM_FLY_DATA					0x02


//功能码   传送长度，接收长度
u8 Protocol[10][3]={
										COM_PID_PARAMS  ,   36   ,		36	,
										COM_FLY_DATA		,		53	 ,		1		,
					};

					
	
//数据结构
struct PID
{
	int16_t pid_param[18];
};
struct FLY_DATA
{
	uint16_t Seq;
	int16_t pitch;
	int16_t roll;	
	int16_t yaw;
	int16_t Acc_x;  
	int16_t Acc_y;
	int16_t Acc_z;
	int16_t Gyro_x;
	int16_t Gyro_y;
	int16_t Gyro_z;
	int16_t Mag_x;
	int16_t Mag_y;
	int16_t Mag_z;
	int16_t Press;
	int16_t optical_flow_x;
	int16_t optical_flow_y;
	int16_t laser_distance;
	uint8_t Fly_state;
};

float accept[18];


UART com1(1,115200);   //定义串口号，波特率

UIDLink _UIDLink(com1,Protocol,5,0xAA,0xAF);	//定义传输帧结构

// Data  
PID 					_params;
FLY_DATA			_flydata;

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
	u8 h_1000Hz = SysTime::CreateTimer(0.01);//定义定时器
	com1.Precision(1);
	uint16_t n=0;
	LED0=1;
	LED1=1;
	u8 keyled1=0;
	u8 keyled2=0;
	u8 _fly_state=0;



while(1){		
			if(SysTime::CheckTimer(h_1000Hz)){
      count++;		
      if(count>=1000){
				count=1;
			}
			cnt++;
			if(cnt>=3)
			{
				if(keyled1==1)
				{
				  LED0=1-LED0;
				}
				else
				{
					LED0=1;
				}
				cnt=0;
			}
			if(cnt==1){
			_params.pid_param[0] = (int16_t)(count);
			_params.pid_param[1] = (int16_t)(count);
			_params.pid_param[2] = (int16_t)(count);
			_params.pid_param[3] = (int16_t)(count);
			_params.pid_param[4] = (int16_t)(count);
			_params.pid_param[5] = (int16_t)(count);
			_params.pid_param[6] = (int16_t)(count);
			_params.pid_param[7] = (int16_t)(count);
			_params.pid_param[8] = (int16_t)(count);
			_params.pid_param[9] = (int16_t)(count);
			_params.pid_param[10] = (int16_t)(count);
			_params.pid_param[11] = (int16_t)(count);
			_params.pid_param[12] = (int16_t)(count);
			_params.pid_param[13] = (int16_t)(count);
			_params.pid_param[14] = (int16_t)(count);
			_params.pid_param[15] = (int16_t)(count);
			_params.pid_param[16] = (int16_t)(count);
			_params.pid_param[17] = (int16_t)(count);
			_UIDLink._Tx_Frame._FunctionCode = COM_PID_PARAMS;
			_UIDLink.CopyFromTxData(_params);
			_UIDLink.Pack_Send();
			}
			count2++;
			if(count2>100)
			{
				count2=0;
			}
			if(cnt==2)
			{
			_flydata.Seq = n++;//传递包数量
			_flydata.roll = (int16_t)(count2);
			_flydata.pitch = (int16_t)(count2);
			_flydata.yaw = (int16_t)(count2);
			_flydata.Acc_x = (int16_t)(count2);
			_flydata.Acc_y = (int16_t)(count2);
			_flydata.Acc_z = (int16_t)(count2);
			_flydata.Gyro_x = (int16_t)(count2);
			_flydata.Gyro_y = (int16_t)(count2);
			_flydata.Gyro_z = (int16_t)(count2);
			_flydata.Mag_x = (int16_t)(count2);
			_flydata.Mag_y = (int16_t)(count2);
			_flydata.Mag_z = (int16_t)(count2);
			_flydata.Press =(int16_t)(count2);
			_flydata.optical_flow_x = 0;
			_flydata.optical_flow_y = 0;
			_flydata.laser_distance = 0;
			_flydata.Fly_state = 0;
			
			_UIDLink._Tx_Frame._FunctionCode = COM_FLY_DATA;
			_UIDLink.CopyFromTxData(_flydata);
			_UIDLink.Pack_Send();
			}
			
		int fnCode =_UIDLink.Pack_Check();
	  if(fnCode>=0){
		  switch(fnCode){
			 case COM_PID_PARAMS:
				accept[0] = (float)(_UIDLink.GetSigned16(0)) / 1000;//角度
				accept[1] = (float)(_UIDLink.GetSigned16(2) )/ 1000;
				accept[2] = (float)(_UIDLink.GetSigned16(4) )/ 1000;
				accept[3]= (float)(_UIDLink.GetSigned16(6) )/ 1000;
				accept[4] = (float)(_UIDLink.GetSigned16(8) )/ 1000;
				accept[5] = (float)(_UIDLink.GetSigned16(10) )/ 1000;
				accept[6] = (float)(_UIDLink.GetSigned16(12) )/ 1000;
				accept[7] = (float)(_UIDLink.GetSigned16(14) )/ 1000;
				accept[8] = (float)(_UIDLink.GetSigned16(16) )/ 1000;
				accept[8] = (float)(_UIDLink.GetSigned16(18)) / 1000;//角速度
				accept[10] = (float)(_UIDLink.GetSigned16(20) )/ 1000;
				accept[11] = (float)(_UIDLink.GetSigned16(22) )/ 1000;
				accept[12] = (float)(_UIDLink.GetSigned16(24) )/ 1000;
				accept[13] = (float)(_UIDLink.GetSigned16(26) )/ 1000;
				accept[14] = (float)(_UIDLink.GetSigned16(28) )/ 1000;
				accept[15] = (float)(_UIDLink.GetSigned16(30) )/ 1000;
				accept[16] = (float)(_UIDLink.GetSigned16(32) )/ 1000;
				accept[17] = (float)(_UIDLink.GetSigned16(34) )/ 1000;
			  keyled1=1;
				break;	
			case COM_FLY_DATA:
				keyled1=0;
				_fly_state = _UIDLink.GetUnSigned8(0);break;
		}	
	}

		}
	}

}


