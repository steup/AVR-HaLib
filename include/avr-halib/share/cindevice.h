/** \addtogroup share */
/*@{*/
/**
 *	\file	avr-halib/share/cindevice.h
 *	\brief	Defines CInDevice
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include <stdint.h>



/**
 *	\class	CInDevice cindevice.h include/avr-halib/share/cindevice.h
 *	\brief	Extends BaseClass with ability to read strings and integers
 *	\see	doc_cdevices
 *	\param	BaseClass	A Class with a method <tt>bool get(char & c)</tt> that returns true if a character c was read.
 */
template <class BaseClass>
		class CInDevice : public BaseClass
{

protected:
	bool isNumber(char c)
	{
		return c >= '0' && c <= '9';
	}
	
	bool isWhitespace(char c)
	{
		return c == ' ' || c == '\n' || c == '\r' || c == '\t';
	}
	
public:

	/**
	 *	\brief	Reads a word (string with no whitespaces inside)
	 *	\param	s	String buffer (string read will be null-terminated)
	 *	\param	maxLength	Length of the string buffer
	 *	\returns	Number of characters read (less than maxLength)
	 *
	 *	This function removes leading whitespaces and the whitespace behind the word.
	 *	
	 *
	 */
	uint8_t readString(char * s, uint8_t maxLength)
	{
		uint8_t r = 0;	// Number of characters read
		bool valid;	// true if the last call of get() was successful
		char c;
		maxLength--;	// Add a string terminating zero at the end
		
		// Eat whitespaces
		while ((valid = BaseClass::get(c)) && isWhitespace(c))
			;
		
		while (valid && !isWhitespace(c) && r < maxLength)
		{
			s[r] = c;
			r++;
			valid = BaseClass::get(c);
		}
		s[r] = 0;
		
		return r;
	}


	/**
	 *	\brief	Read a number
	 *	\param	val	Reference to a variable to store the parsed number
	 *	\returns	true, if a number was found
	 *
	 *	This function removes leading whitespaces.
	 *
	 *	\attention The first character behind the number or (if there is no number) behind the last
	 *	of leading whitespaces is lost! Hint: use a clear syntax for data transmitted though interfaces
	 *	(e.g. a keyword in front of numbers and a whitespace behind a number).
	 *	\attention If the number is bigger than the maximum value of val, this function returns wrong val!
	 */
	bool readInt(int32_t & val)
	{
		bool numberFound = false;
		bool neg = false;	// negative number
		bool valid;		// true if the last call of get() was successful
		char c;
		val = 0;
		
		// Eat whitespaces
		while ((valid = BaseClass::get(c)) && isWhitespace(c))
			;
	
		if (valid && c == '-')
		{
			neg = true;
			valid = BaseClass::get(c);
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
				
			valid = BaseClass::get(c);
		}
		
		if (neg)
			val = -val;
		
		return numberFound;
	}
};

template <class Base,class T> class CInDeviceTypeWrap:public Base
{
	public:
	bool get(T & c){return Base::get((char &)(c));}
};


/*@}*/
