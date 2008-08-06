/**
 *	\file	exapmles/application/delay.cpp
 *	\brief	Example for busy waiting
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include <avr/io.h>


#define F_CPU 16000000

#include "avr-halib/share/delay.h"



int main()
{
	DDRA |= 1;
	while (1)
	{
		PORTA ^= 1;
		delay_ms(1000);
	}
}
