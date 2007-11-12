/**	\file halib/cdevice.h
 *
 *	\brief	Definiert Basisklasse für Char-Devices.
 *	\see 	halibcdevices
 *
 */

#pragma once

#include "config.h"

/*!	\brief Basisklasse für Char-Devices
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

	/// Out-Streaming-Operator für Strings
	CDevice & operator<<(const char * c)
	{
		sout(c);
		return *this;
	}

	/// Out-Streaming-Operator für Integers
	CDevice & operator<<(int32_t d)
	{
		iout(d);
		return *this;
	}

};
