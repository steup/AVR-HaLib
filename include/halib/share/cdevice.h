/**	\file halib/share/cdevice.h
 *
 *	\brief	Defines a base class for character devices
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 */

#pragma once

#include "halib/config.h"

/*!	\brief	Base class for character devices
*	\see	doc_cdevices
*/
class CDevice
{
	
public:

	/// Write a character
	virtual void putc(const char c) = 0;
	
	/// Read a character
	virtual char getc() = 0;

	/// Write a string
	void writeString(const char * c);
	
	/// Write an integer
	void writeInt(int32_t d);

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
	uint8_t readString(char * s, uint8_t maxLength);

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
	*/
	bool readInt(int32_t & val);

	/// Write a newline
	void newline();

	/// Streaming operator for string output
	CDevice & operator<<(const char * c)
	{
		writeString(c);
		return *this;
	}

	/// Streaming operator for integer output
	CDevice & operator<<(int32_t d)
	{
		writeInt(d);
		return *this;
	}

};
