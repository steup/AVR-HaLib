#pragma once

#include "halib/cdevice.h"

/*! \brief Das Null-Char-Device
 * 
 *	alle Eingaben gehen verlohren, es werden nur nullen ausgegeben
 */
class CNull : public CDevice
{

public:
	CNull(){}
	/**
	 * putc Schiebt ein Zeichen weg
	 */
	void putc (const char c){}

	/**
	 * getc Liest gibt 0 zur�ck 
	 */ 
	char getc()
	{
		return 0;
	}

};