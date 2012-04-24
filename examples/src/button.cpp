/**
 *	\file	examples/applications/button.cpp
 *	\brief	Example for usage of Button and Led (very simple)
 *	
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "platform.h"

#include "avr-halib/ext/button.h"
#include "avr-halib/ext/led.h"

using avr_halib::ext::Button;
using avr_halib::ext::Led;

int main()
{
	Button<Button0> button0;
	Button<Button1> button1;
	Button<Button2> button2;
	Button<Button3> button3;
	Led<Led0> led0;
	Led<Led1> led1;
	Led<Led2> led2;
	Led<Led3> led3;

	while (1)
	{

 		led0.set(button0.isPressed());

 		if (button1.isPressed())
			led1.setOn();
 		else
			led1.setOff();

 		if (button2.isPressedWait())
 			led2.toggle();
		
 		if (button3.gotPressed())
			led3.toggle();
	}
}
