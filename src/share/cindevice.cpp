/**
 *	\file	halib/share/cindevice.cpp
 *
 *	\brief	Implements base class for char input devices
 *	\author	Philipp Werner
 */

#include "halib/share/cindevice.h"


bool isNumber(char c)
{
	return c >= '0' && c <= '9';
}

bool isWhitespace(char c)
{
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}


uint8_t CInDevice::readString(char * s, uint8_t maxLength)
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

bool CInDevice::readInt(int32_t & val)
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
