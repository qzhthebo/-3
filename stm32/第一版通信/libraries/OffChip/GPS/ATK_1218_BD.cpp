#include "ATK_1218_BD.h"

const u32 BAUD_id[9]=
{4800,9600,19200,38400,57600,
115200,230400,460800,921600};// Baud Sequence that the Module support


ATK_1218_BD::ATK_1218_BD(UART &COM):_COM(COM),_Rx_Buffer(COM._RX_Buffer){
	debug = false;
}

u8 ATK_1218_BD::NMEA_Comma_Pos(u8 *Buf,u8 Num){
	u8 *p = Buf;
	while(Num){
		if(*Buf=='*'||*Buf<' '||*Buf>'z')
			return 0xFF;
		if(*Buf==',')
			Num--;
		Buf++;
	}
	return Buf-p;

}	
u8 ATK_1218_BD::NMEA_Pow(u8 m,u8 n){
	u32 result = 1;
	while(n--)
		result *= m;
	return result;
}

int ATK_1218_BD::NMEA_StingToNum(u8 *Buf,u8 *Dx){
	u8 *p = Buf;
	u8 Int_Len=0,Floa_Len=0;
	u8 Int_Res=0,Floa_Res=0;
	u8 Mask=0;
	int Res=0;
	while(1){
		if(*p=='-'){
			Mask|=0x02;
			p++;
		}
		if(*p==','||*p=='*')
			break;
		if(*p=='.'){
			Mask|=0x01;
			p++;
		}else if((*p>'9')||(*p<'0')){
			Int_Len=0;
			Floa_Len=0;
			break;
		}
		if(Mask&0x01)
			Floa_Len++;
		else
			Int_Len++;
		p++;
	}
	if(Mask&0x02)
		Buf++;
	for(u8 i=0;i<Int_Len;i++)
		Int_Res += NMEA_Pow(10,Int_Len-1-i)*(Buf[i]-'0');
	if(Floa_Len>5) Floa_Len=5;
	*Dx=Floa_Len;
	for(u8 i=0;i<Floa_Len;i++)
		Floa_Res += NMEA_Pow(10,Floa_Len-1-i)*(Buf[Int_Len+1+i]-'0');
	Res = Int_Res*NMEA_Pow(10,Floa_Len)+Floa_Res;
	if(Mask&0x02)
		Res=-Res;
	return Res;
}


void ATK_1218_BD::GPS_Analysis(NMEA_MSG *Gpsx,u8 *buf){
	NMEA_GPGSV_Analysis(Gpsx,buf);	//GPGSV Analysis
	NMEA_BDGSV_Analysis(Gpsx,buf);	//BDGSV Analysis
	NMEA_GNGGA_Analysis(Gpsx,buf);	//GNGGA Analysis
	NMEA_GNGSA_Analysis(Gpsx,buf);	//GPNSA Analysis
	NMEA_GNRMC_Analysis(Gpsx,buf);	//GPNMC Analysis
	NMEA_GNVTG_Analysis(Gpsx,buf);	//GPNTG Analysis
}


void ATK_1218_BD::NMEA_GPGSV_Analysis(NMEA_MSG* Gpsx,u8 *buf){
	u8 *p,*p_Sub,dx;
	u8 len,i,j,slx=0;
	u8 posX;
	p=buf;
	p_Sub=(u8*)strstr((const char*)p,"$GPGSV");
	len=p_Sub[7]-'0';
	posX = NMEA_Comma_Pos(p_Sub,3);
	if(posX!=0xFF)
		Gpsx->visibleSatellite_Num = NMEA_StingToNum(p_Sub,&dx);
	for(i=0;i<len;i++){
		p_Sub=(u8*)strstr((const char*)p,"$GPGSV");		
		for(j=0;j<4;j++){
			posX=NMEA_Comma_Pos(p_Sub,4+j*4);
			if(posX!=0xFF)
				Gpsx->SLMSG[slx].Number = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			posX=NMEA_Comma_Pos(p_Sub,5+j*4);
			if(posX!=0xFF)
				Gpsx->SLMSG[slx].ELEDEG = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			posX=NMEA_Comma_Pos(p_Sub,6+j*4);
			if(posX!=0xFF)
				Gpsx->SLMSG[slx].AZIDEG = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			posX=NMEA_Comma_Pos(p_Sub,7+j*4);
			if(posX!=0xFF)
				Gpsx->SLMSG[slx].SN = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			slx++;			
		}
		p=p_Sub+1;
	}
}


void ATK_1218_BD::NMEA_BDGSV_Analysis(NMEA_MSG* Gpsx,u8 *buf){
	u8 *p,*p_Sub,dx;
	u8 len,i,j,slx=0;
	u8 posX;
	p=buf;
	p_Sub=(u8*)strstr((const char*)p,"$BDGSV");
	len=p_Sub[7]-'0';
	posX = NMEA_Comma_Pos(p_Sub,3);
	if(posX!=0xFF)
		Gpsx->visibleSatellite_Num = NMEA_StingToNum(p_Sub,&dx);
	for(i=0;i<len;i++){
		p_Sub=(u8*)strstr((const char*)p,"$BDGSV");		
		for(j=0;j<4;j++){
			posX=NMEA_Comma_Pos(p_Sub,4+j*4);
			if(posX!=0xFF)
				Gpsx->BDS_SLMSG[slx].BDS_Number = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			posX=NMEA_Comma_Pos(p_Sub,5+j*4);
			if(posX!=0xFF)
				Gpsx->BDS_SLMSG[slx].BDS_ELEDEG = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			posX=NMEA_Comma_Pos(p_Sub,6+j*4);
			if(posX!=0xFF)
				Gpsx->BDS_SLMSG[slx].BDS_AZIDEG = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			posX=NMEA_Comma_Pos(p_Sub,7+j*4);
			if(posX!=0xFF)
				Gpsx->BDS_SLMSG[slx].BDS_SN = NMEA_StingToNum(p_Sub+posX,&dx);
			else break;
			slx++;			
		}
		p=p_Sub+1;
	}	
}


void ATK_1218_BD::NMEA_GNGGA_Analysis(NMEA_MSG* Gpsx,u8 *buf){
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNGGA");
	posx=NMEA_Comma_Pos(p1,6);								
	if(posx!=0XFF)
		Gpsx->GPS_Status=NMEA_StingToNum(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);							
	if(posx!=0XFF)
		Gpsx->Pos_Satellite_Num=NMEA_StingToNum(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);							
	if(posx!=0XFF)
		Gpsx->Altitude=NMEA_StingToNum(p1+posx,&dx);	
}


void ATK_1218_BD::NMEA_GNGSA_Analysis(NMEA_MSG* Gpsx,u8 *buf){
	u8 *p1,dx;			 
	u8 posx; 
	u8 i;   
	p1=(u8*)strstr((const char *)buf,"$GNGSA");
	posx=NMEA_Comma_Pos(p1,2);							
	if(posx!=0XFF)Gpsx->FixMode=NMEA_StingToNum(p1+posx,&dx);	
	for(i=0;i<12;i++){
		posx=NMEA_Comma_Pos(p1,3+i);					 
		if(posx!=0XFF)
			Gpsx->Pos_Satellite[i]=NMEA_StingToNum(p1+posx,&dx);
		else break; 
	}				  
	posx=NMEA_Comma_Pos(p1,15);								
	if(posx!=0XFF)
		Gpsx->PDOP=NMEA_StingToNum(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,16);							
	if(posx!=0XFF)
		Gpsx->HDOP=NMEA_StingToNum(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,17);								
	if(posx!=0XFF)
		Gpsx->VDOP=NMEA_StingToNum(p1+posx,&dx);  	
	
}


void ATK_1218_BD::NMEA_GNRMC_Analysis(NMEA_MSG* Gpsx,u8 *buf){
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs;  
	p1=(u8*)strstr((const char *)buf,"$GNRMC");
	posx=NMEA_Comma_Pos(p1,1);							
	if(posx!=0XFF)
	{
		temp=NMEA_StingToNum(p1+posx,&dx)/NMEA_Pow(10,dx);	 	
		Gpsx->UTC.hour=temp/10000;
		Gpsx->UTC.min=(temp/100)%100;
		Gpsx->UTC.sec=temp%100;	 	 
	}	
	posx=NMEA_Comma_Pos(p1,3);								
	if(posx!=0XFF){
		temp=NMEA_StingToNum(p1+posx,&dx);		 	 
		Gpsx->Latitude=temp/NMEA_Pow(10,dx+2);	
		rs=temp%NMEA_Pow(10,dx+2);					 
		Gpsx->Latitude=Gpsx->Latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;
	}
	posx=NMEA_Comma_Pos(p1,4);							
	if(posx!=0XFF)Gpsx->NS_Latitude=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);							
	if(posx!=0XFF){												  
		temp=NMEA_StingToNum(p1+posx,&dx);		 	 
		Gpsx->Longitude=temp/NMEA_Pow(10,dx+2);
		rs=temp%NMEA_Pow(10,dx+2);			 
		Gpsx->Longitude=Gpsx->Longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60; 
	}
	posx=NMEA_Comma_Pos(p1,6);							
	if(posx!=0XFF)Gpsx->NS_Longitude=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);						
	if(posx!=0XFF){
		temp=NMEA_StingToNum(p1+posx,&dx);		 			
		Gpsx->UTC.date=temp/10000;
		Gpsx->UTC.month=(temp/100)%100;
		Gpsx->UTC.year=2000+temp%100;	 	 
	} 	
}


void ATK_1218_BD::NMEA_GNVTG_Analysis(NMEA_MSG* Gpsx,u8 *buf){
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								
	if(posx!=0XFF){
		Gpsx->Speed=NMEA_StingToNum(p1+posx,&dx);
		if(dx<3)Gpsx->Speed*=NMEA_Pow(10,3-dx);	 	 		
	}	
}
u8 ATK_1218_BD::GPS_Update(NMEA_MSG *Gpsx){
	u8 temp[500];
	u8 tmp;
	if((tmp=SkyTra_CFG_Rate(5))!=0){
			return tmp;
	}
	_Rx_Buffer.GetDatas(temp,500);
	for(u16 i=0;i<500;i++)
	{
		if(temp[i]!=0){
				GPS_Analysis(Gpsx,temp);
				return 0;		
		}
	}
}

u8 ATK_1218_BD::SkyTra_CFG_Ack_Check(void){
	u16 len=0;
	u8 ERROR_Code = 0;
	u16 i;
	bool flag = false;
	debug = false;
	while(_Rx_Buffer._size_<500 &&!flag && len<50000){
		len++;
		if(++_Rx_Buffer._index_out >= _Rx_Buffer._max_size_)
			_Rx_Buffer._index_out = 0;
		_Rx_Buffer._size_--;
		if(_Rx_Buffer._buffer[_Rx_Buffer._index_out]=='$')
			flag = true;
	}
	
	if(len<500){
		for(i=0;i<_Rx_Buffer._size_;i++){
			if(_Rx_Buffer._buffer[_Rx_Buffer._index_out+i]==0X83){
				debug = true;
				break;
			}
			else if(_Rx_Buffer._buffer[_Rx_Buffer._index_out+i]==0X84){
				ERROR_Code=3;
				break;
			}
		}
		if(i==_Rx_Buffer._size_)
			ERROR_Code=i;
	}else
		ERROR_Code=1;
	return ERROR_Code;
}

u8 ATK_1218_BD::SkyTra_CFG_Port(u32 Baud_ID){
	SkyTra_Baud *cfg_prt;
	cfg_prt->Start_Sequence=0XA1A0;		
	cfg_prt->Len_Valid=0X0400;			
	cfg_prt->ID=0X05;		    
	cfg_prt->COM_Port=0X00;			
	cfg_prt->Baud_id=Baud_ID;	 	
	cfg_prt->Attributes=1; 		 
	cfg_prt->Check=cfg_prt->ID^cfg_prt->COM_Port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->End=0X0A0D;      
	SkyTra_Send_Date((u8*)cfg_prt,sizeof(SkyTra_Baud));	
	SysTime::delay(0.2);
	return SkyTra_CFG_Ack_Check();
}

u8 ATK_1218_BD::SkyTra_CFG_Tp(u32 width){
	u32 temp=width;
	SkyTra_PPS_WIDTH *cfg_tp;
	temp=(width>>24)|((width>>8)&0X0000FF00)|((width<<8)&0X00FF0000)|((width<<24)&0XFF000000);//小端模式
	cfg_tp->Start_Sequence=0XA1A0;		    //cfg header(小端模式)
	cfg_tp->Len_Valid=0X0700;        //有效数据长度(小端模式)
	cfg_tp->ID=0X65	;			    //cfg tp id
	cfg_tp->Sub_ID=0X01;			//数据区长度为20个字节.
	cfg_tp->width=temp;		  //脉冲宽度,us
	cfg_tp->Attributes=0X01;  //保存到SRAM&FLASH	
	cfg_tp->Check=cfg_tp->ID^cfg_tp->Sub_ID^(cfg_tp->width>>24)^(cfg_tp->width>>16)&&0XFF^(cfg_tp->width>>8)&&0XFF^cfg_tp->width&&0XFF^cfg_tp->Attributes;    	//用户延时为0ns
	cfg_tp->End=0X0A0D;       //发送结束符(小端模式)
	SkyTra_Send_Date((u8*)cfg_tp,sizeof(SkyTra_PPS_WIDTH));//发送数据给NEO-6M  
	return SkyTra_CFG_Ack_Check();	
	
}


u8 ATK_1218_BD::SkyTra_CFG_Rate(u8 Frep){
	SkyTra_PosRate *cfg_rate=(SkyTra_PosRate*)buffer;
 	cfg_rate->Start_Sequence=0XA1A0;	    //cfg header(小端模式)
	cfg_rate->Len_Valid=0X0300;			//有效数据长度(小端模式)
	cfg_rate->ID=0X0E;	      //cfg rate id
	cfg_rate->Rate=Frep;	 	  //更新速率
	cfg_rate->Attributes=0X01;	   	//保存到SRAM&FLASH	.
	cfg_rate->Check=cfg_rate->ID^cfg_rate->Rate^cfg_rate->Attributes;//脉冲间隔,us
	cfg_rate->End=0X0A0D;       //发送结束符(小端模式)
	SkyTra_Send_Date((u8*)cfg_rate,sizeof(SkyTra_PosRate));//发送数据给NEO-6M 
	return SkyTra_CFG_Ack_Check();
}
void ATK_1218_BD::SkyTra_Send_Date(u8 *DBuf,u16 len){
	_COM.SendBytes(DBuf,len);
}

