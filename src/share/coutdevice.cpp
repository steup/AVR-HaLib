/**
 *	\file	halib/share/coutdevice.cpp
 *
 *	\brief	Implements base class for char output devices
 *	\author	Philipp Werner
 */

#include "halib/share/coutdevice.h"

void COutDevice::newline()
{
	putc('\n');
	putc('\r');
}

void COutDevice::writeString(const char * c)
{
	while (*c)
		putc(*(c++));
}

void COutDevice::writeInt(int32_t d)
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

		uint8_t i = 11;		// position in buffer
		buffer[11] = '\0';
		while (d)
		{
			buffer[--i] = '0' + (d % 10);
			d /= 10;
		}

		if (neg)
			buffer[--i] = '-';
		writeString(buffer+i);
	}
}
