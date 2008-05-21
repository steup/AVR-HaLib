/**
 *	\file	examples/applications/button.cpp
 *	\brief	Example for usage of Button and Led (very simple)
 *	
 */

#include "halib/ext/button.h"
#include "halib/ext/led.h"
#include "halib/ext/ledblock.h"


#include "halib/portmaps/robbyboard.h"

int main()
{
	Button<Button0> button0;
	Button<Button1> button1;
	Button<Button2> button2;
	Button<Button3> button3;
// 	Led<Led0> led0;
// 	Led<Led1> led1;
// 	Led<Led2> led2;
// 	Led<Led3> led3;
	LedBlock<LedBlock0123> leds;
	uint8_t patter = 0;

	while (1)
	{
		
// 		led0.set(button0.isPressed());
 		if (button0.isPressed())
			patter |= 1;
// 			led0.setOn();
 		else
			patter &= ~1;
// 			led0.setOff();
 		if (button1.isPressed())
			patter |= 2;
// 			led1.setOn();
 		else
			patter &= ~2;
// 			led1.setOff();
 		if (button2.isPressed())
			patter |= 4;
// 			led2.setOn();
 		else
			patter &= ~4;
// 			led2.setOff();
 		if (button3.isPressed())
			patter |= 8;
// 			led3.setOn();
 		else
			patter &= ~8;

// 			led3.setOff();
// 		led1.set(button1.isPressedWait());
// 		led2.set(button2.gotPressed());
// 		led3.set(button3.isPressed());
leds.set(patter);
	}
}
