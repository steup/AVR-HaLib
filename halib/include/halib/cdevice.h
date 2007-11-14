/**	\file halib/cdevice.h
 *
 *	\brief	Definiert Basisklasse f�r Char-Devices.
 *	\see 	halibcdevices
 *
 */

#pragma once

#include "halib/config.h"

/*!	\brief Basisklasse f�r Char-Devices
*
*/
class CDevice
{
	
public:

	/// Zeichen ausgeben
	virtual void putc(const char c) = 0;
	
	/// Zeichen auslesen
	virtual char getc() = 0;

	/// String ausgeben
	void sout(const char * c);
	
	/// Integer ausgeben
	void iout(int32_t d);
	
	/// Newline ausgeben
	void newline();

	/// Out-Streaming-Operator f�r Strings
	CDevice & operator<<(const char * c)
	{
		sout(c);
		return *this;
	}

	/// Out-Streaming-Operator f�r Integers
	CDevice & operator<<(int32_t d)
	{
		iout(d);
		return *this;
	}

};
