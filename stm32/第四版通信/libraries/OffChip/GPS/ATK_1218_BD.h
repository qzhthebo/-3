#ifndef _ATK_1218_BD_H
#define _ATK_1218_BD_H

#include "stm32f10x.h"
#include "string.h"
#include "UART.h"
#include "SysTime.h"



//GPS NMEA-0813 Protocol params structure
__packed typedef struct{
	u8 Number;
	u8 ELEDEG;
	u16 AZIDEG;
	u8 SN;
}NMEA_SLMSG;

//BDS NMEA-0813 Protocol params structure
__packed typedef struct{
	u8 BDS_Number;
	u8 BDS_ELEDEG;
	u16 BDS_AZIDEG;
	u8 BDS_SN;
}BDS_NMEA_SLMSG;

//UTC Time Data structure
__packed typedef struct{
	u16 year;
	u8 month;
	u8 date;
	u8 hour;
	u8 min;
	u8 sec;
}NMEA_UTC_TIME;

//NMEA-0812 All Data structure
__packed typedef struct{
	u8 visibleSatellite_Num;
	u8 visibleSatelliteBDS_Num;
	NMEA_SLMSG SLMSG[12];
	BDS_NMEA_SLMSG BDS_SLMSG[12];
	NMEA_UTC_TIME UTC;
	u32 Latitude;
	u8 NS_Latitude;
	u32 Longitude;
	u8 NS_Longitude;
	u8 GPS_Status;
	u8 Pos_Satellite_Num;
	u8 Pos_Satellite[12];
	u8 FixMode;
	u16 PDOP;
	u16 HDOP;
	u16 VDOP;
	
	int Altitude;
	u16 Speed;
}NMEA_MSG;

//SkyTra S1216F8 SetBaud struceture
__packed typedef struct
{
	u16 Start_Sequence;          
	u16 Len_Valid;             
	u8 ID;           
	u8 COM_Port;          
	u8 Baud_id;      
	u8 Attributes;     
	u8 Check;         
	u16 End;          
}SkyTra_Baud;

//SkyTra S1216F8 Set Output Data structure
__packed typedef struct
{
	u16 Start_Sequence;          
	u16 Len_Valid;             
	u8 ID; 
	u8 GGA;           
	u8 GSA;         
	u8 GSV;          
	u8 GLL;           
	u8 RMC;           
	u8 VTG;           
	u8 ZDA;           
	u8 Attributes;     
	u8 Check;             
	u16 End;            
}SkyTra_OutMSG;

//SkyTra S1216F8 Set Pos UpdateSpeed structure
__packed typedef struct
{
	u16 Start_Sequence;          
	u16 Len_Valid;             
	u8 ID; 
	u8 Rate;           // in range:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	u8 Attributes;     
	u8 Check;            
	u16 End;            
}SkyTra_PosRate;

//SkyTra S1216F8 Set Output PulseWidth structure
__packed typedef struct
{
	u16 Start_Sequence;          
	u16 Len_Valid;             
	u8 ID; 
	u8 Sub_ID;         
	u32 width;        
	u8 Attributes;     
	u8 Check;             
	u16 End;            
}SkyTra_PPS_WIDTH;






class ATK_1218_BD{
private:
		UART& _COM;
		FIFOBuffer<u8,500> &_Rx_Buffer;
		u8 buffer[500];
public:
	NMEA_MSG Msg;
	bool debug;
	ATK_1218_BD(UART &COM);
	u8 GPS_Update(NMEA_MSG *Gpsx);
private:
	u8 NMEA_Comma_Pos(u8 *Buf,u8 Num);	
	u8 NMEA_Pow(u8 m,u8 n); 
	int NMEA_StingToNum(u8 *Buf,u8 *Dx);
	void GPS_Analysis(NMEA_MSG *Gpsx,u8 *buf);
	void NMEA_GPGSV_Analysis(NMEA_MSG* Gpsx,u8 *buf);
	void NMEA_BDGSV_Analysis(NMEA_MSG* Gpsx,u8 *buf);
	void NMEA_GNGGA_Analysis(NMEA_MSG* Gpsx,u8 *buf);
	void NMEA_GNGSA_Analysis(NMEA_MSG* Gpsx,u8 *buf);
	void NMEA_GNRMC_Analysis(NMEA_MSG* Gpsx,u8 *buf);
	void NMEA_GNVTG_Analysis(NMEA_MSG* Gpsx,u8 *buf);
	u8 SkyTra_CFG_Ack_Check(void);
	u8 SkyTra_CFG_Port(u32 Baud_ID);
	u8 SkyTra_CFG_Tp(u32 width);
	u8 SkyTra_CFG_Rate(u8 Frep);
	void SkyTra_Send_Date(u8 *DBuf,u16 len);
};


#endif
