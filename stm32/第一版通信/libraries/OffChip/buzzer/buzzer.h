#include "State.h"
#include "GPIO.h"
#include "stm32f10x.h"
class buzzer{
	public:
		GPIO p;
		buzzer(GPIO &_p);
		void bi();
		void bi(int n);
};
