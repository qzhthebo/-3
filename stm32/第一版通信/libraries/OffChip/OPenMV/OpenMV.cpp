#include"OpenMV.h"
int OpenMV::update(){
	int fncode = 0;
			int fnCode =_UIDLink.Pack_Check();
			if(fncode == 0x19){
				 distance = _UIDLink.GetFloat(0);
				 cx = _UIDLink.GetUnSigned16(4);
				 cy = _UIDLink.GetUnSigned16(6);
				 lx1  = _UIDLink.GetUnSigned16(8);
				 lx2  = _UIDLink.GetUnSigned16(10);				
			}
			return fncode;
}

