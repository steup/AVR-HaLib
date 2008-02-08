/**
 *	\file halib/cdevice.cpp
 *
 *	\brief	Implementiert Basisklasse f�r Char-Devices.
 *
 */

#include "halib/cdevice.h"

void CDevice::newline()
{
	putc('\n');
	putc('\r');
}

void CDevice::sout(const char * c)
{
	while (*c)
		putc(*(c++));
}

void CDevice::iout(int32_t d)
{
	if (d == 0)
	{
		putc('0');
	}
	else
	{
		bool neg = false;
		if (d < 0)
		{
		    neg = true;
		    d = -d;
		}
		
		char buffer [12];

		uint8_t i = 11;		// aktuelle Position in buffer
		buffer[11] = '\0';
		while (d)
		{
			buffer[--i] = '0' + (d % 10);
			d /= 10;
		}

		if (neg)
			buffer[--i] = '-';
		sout(buffer+i);
	}
}
