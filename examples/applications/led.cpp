
#include "halib/ext/led.h"
#include "halib/portmaps/robbyboard.h"

Led<Led0> led0;

int main() 
{
//Led<Led0> led0;
	led0.setOn();
	led0.setOff();
	if (led0.isOn())
		led0.toggle();

	while (1);
	return 0;	
}

