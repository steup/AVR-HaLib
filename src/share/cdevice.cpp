/**
 *	\file	halib/share/cdevice.cpp
 *
 *	\brief	Implements base class for char devices
 *	\author	Philipp Werner
 */

#include "halib/share/cdevice.h"

void CDevice::newline()
{
	putc('\n');
	putc('\r');
}

void CDevice::writeString(const char * c)
{
	while (*c)
		putc(*(c++));
}

void CDevice::writeInt(int32_t d)
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


bool isNumber(char c)
{
	return c >= '0' && c <= '9';
}

bool isWhitespace(char c)
{
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}


uint8_t CDevice::readString(char * s, uint8_t maxLength)
{
	uint8_t r = 0;	// Number of characters read
	bool valid;	// true if the last call of getc() was successful
	char c;
	maxLength--;	// Add a string terminating zero at the end
	
	// Eat whitespaces
	while ((valid = getc(c)) && isWhitespace(c))
		;
	
	while (valid && !isWhitespace(c) && r < maxLength)
	{
		s[r] = c;
		r++;
		valid = getc(c);
	}
	s[r] = 0;
	
	return r;
}

bool CDevice::readInt(int32_t & val)
{
	bool numberFound = false;
	bool neg = false;	// negative number
	bool valid;		// true if the last call of getc() was successful
	char c;
	val = 0;
	
	// Eat whitespaces
	while ((valid = getc(c)) && isWhitespace(c))
		;

	if (valid && c == '-')
	{
		neg = true;
		valid = getc(c);
	}
	
	while (valid)
	{
		if (!isNumber(c))	// c is no diget -> end of number
			break;
		else
		{
			val *= 10;
			val += (c - '0');
			numberFound = true;
		}
			
		valid = getc(c);
	}
	
	if (neg)
		val = -val;
	
	return numberFound;
}
