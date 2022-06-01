

void main()
{
	while(1)
	{
		if(SysTime::CheckTimer(h))
		{
			switch(Time_Slice)
			{
				case 1:
					rc.Update();
					break;
				case 2:
					imu.Update();
					break;
				case 3:
					Safty::Update();
					break;
				case 4:
					Control::Update();
					break;
				case 5:
					Communication::Update();
					break;
				case 6:
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					break;
				case 10:
					break;
			}
		}
	}
} 



void  Control::Update()
{
	switch(SystemSate)
	{
		case stop:
			Stop();
			break;
		case test:
			Test();
			break;
		case auto:
			
			break;
	}
}


