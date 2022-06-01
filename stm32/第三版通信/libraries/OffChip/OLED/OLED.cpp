#include "OLED.h"
#include "GPIO.h"

//SPI _spi(1,SPISpeed::MHz_9,SPI_DataSize_8b);
//GPIO _RES(GPIOC,3,GPIOMode::OUT_PP,GPIOSpeed::HIGH);
//GPIO _DC(GPIOC,4,GPIOMode::OUT_PP,GPIOSpeed::HIGH);
//GPIO _CS(GPIOC,5,GPIOMode::OUT_PP,GPIOSpeed::HIGH);
//OLED _oled(_spi,_RES,_DC,_CS);
OLED::OLED(SPI &_SPIx,GPIO &RES, GPIO &DC,GPIO &CS):_mSPI(_SPIx)
,_RES(RES),_DC(DC),_CS(CS)
{
	_RES = 1; 
	_DC = 1;
	_CS = 1;

	_RES = 0;
	for(u32 i=0;i<30000;i++);
	_RES = 1;
	
	OLED_WriteByte(0xAE,OLED_CMD);//--turn off oled panel   //�ر���Ļ
	OLED_WriteByte(0x00,OLED_CMD);//---set low column address
	OLED_WriteByte(0x10,OLED_CMD);//---set high column address
	OLED_WriteByte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteByte(0x81,OLED_CMD);//--set contrast control register
	OLED_WriteByte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WriteByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WriteByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WriteByte(0xA6,OLED_CMD);//--set normal display
	OLED_WriteByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WriteByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WriteByte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteByte(0x00,OLED_CMD);//-not offset
	OLED_WriteByte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WriteByte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteByte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WriteByte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteByte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WriteByte(0x12,OLED_CMD);
	OLED_WriteByte(0xDB,OLED_CMD);//--set vcomh
	OLED_WriteByte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WriteByte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteByte(0x02,OLED_CMD);//
	OLED_WriteByte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WriteByte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WriteByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 

	OLED_Clear();										// clean the screen
	OLED_WriteByte(0xAF,OLED_CMD); //turn on screen
	
}
void OLED::debug(){
	_CS = 0;
}
void OLED::OLED_WriteByte(u8 data,u8 cmd){
	if(!cmd)
		_DC = 0;
	else 
		_DC = 1;
	
	_CS = 0;
	_mSPI.ReadWriteByte(data);
	_CS = 0;
	_DC = 0;
}

void OLED::OLED_ColorTurn(bool isNormal){
	if(isNormal){
		//normal color display
		OLED_WriteByte(0xA6,OLED_CMD);
	}else{
		//invert color display
		OLED_WriteByte(0xA7,OLED_CMD);
	}
}

void OLED::OLED_DisplayTurn(bool isNormal){
	if(isNormal){
		//normal display
		OLED_WriteByte(0xC8,OLED_CMD);
		OLED_WriteByte(0xA1,OLED_CMD);
	}else{
		//overturn display
		OLED_WriteByte(0xC0,OLED_CMD);
		OLED_WriteByte(0xA0,OLED_CMD);
	}	
}


void OLED::OLED_DisPlay_On(void){
	// Charge Pump enable
	OLED_WriteByte(0x8D,OLED_CMD);
	// turn on Pump charge
	OLED_WriteByte(0x14,OLED_CMD);
	// light the screen
	OLED_WriteByte(0xAF,OLED_CMD);
}

void OLED::OLED_DisPlay_Off(void){
	// Charge Pump enable
	OLED_WriteByte(0x8D,OLED_CMD);
	// turn off Pump charge
	OLED_WriteByte(0x10,OLED_CMD);
	// extinguish the screen
	OLED_WriteByte(0xAE,OLED_CMD);
}

void OLED::OLED_Refresh(void){
	//update the virtual memory to OLED
	for(u8 i=0;i<8;i++){
		//set row begin
		OLED_WriteByte(0xB0+i,OLED_CMD);
		//set column low
		OLED_WriteByte(0x00,OLED_CMD);
		//set column high
		OLED_WriteByte(0x10,OLED_CMD);
		for(u8 n=0;n<128;n++)
			OLED_WriteByte(OLED_GRAM[n][i],OLED_DATA);
	}
}

void OLED::OLED_Clear(void){
	for(u8 i=0;i<8;i++)
		for(u8 n=0;n<128;n++)
			OLED_GRAM[n][i]=0;
	OLED_Refresh();
}


//x:0~127
//y:0~63
//t:1 fill 0,clear	
void OLED::OLED_DrawPoint(u8 x,u8 y,u8 t){
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t) OLED_GRAM[x][i]|=n;
	else{
		OLED_GRAM[x][i] =~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] =~OLED_GRAM[x][i];
	}
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED::OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//x,y:Բ������
//r:Բ�İ뾶
void OLED::OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED::OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //����0806����
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}

//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED::OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//��ʾ����
//x,y :�������
//num :Ҫ��ʾ������
//len :���ֵ�λ��
//size:�����С
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED::OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED::OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//����16*16����
		else if(size1==24)
				{temp=Hzk2[num][i];}//����24*24����
		else if(size1==32)       
				{temp=Hzk3[num][i];}//����32*32����
		else if(size1==64)
				{temp=Hzk4[num][i];}//����64*64����
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED::OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //д��һ�����ֱ�����OLED_GRAM[][]������
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //��ʾ���
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //ʵ������
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y���������
//sizex,sizey,ͼƬ����
//BMP[]��Ҫд���ͼƬ����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED::OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}

