/**	\file halib/share/coutdevice.h
 *
 *	\brief	Defines a base class for character output devices
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 */

#pragma once

#include <stdint.h>

/**
 *	\class	COutDevice coutdevice.h include/halib/share/coutdevice.h
 *	\brief	Base class for character output devices
 *	\see	doc_cdevices
 */
class COutDevice
{
	
public:

	/// Write a character
	virtual void putc(const char c) = 0;

	/// Write a string
	void writeString(const char * c);
	
	/// Write an integer
	void writeInt(int32_t d);

	/// Write a newline
	void newline();

	/// Streaming operator for string output
	COutDevice & operator<<(const char * c)
	{
		writeString(c);
		return *this;
	}

	/// Streaming operator for integer output
	COutDevice & operator<<(int32_t d)
	{
		writeInt(d);
		return *this;
	}

};
