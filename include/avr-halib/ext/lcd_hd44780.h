/** \addtogroup ext */
/*@{*/
/**
 *	\file	include/avr-halib/ext/lcd_hd44780.h
 *	\brief	Defines class LcdHd44780
 *	\author	Karl Fessel, Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/portmap.h"
#include "avr-halib/share/delay.h"
#include "avr-halib/ext/hd44780.h"

/// Use with COutDevice
// TODO: Doku!
template <class LCDPortmap> class LcdHd44780:public Hd44780<LCDPortmap>
{
public:
	
	/// Writes a character to Display
	void put(const char c)
	{
		Hd44780<LCDPortmap>::write(c, true, false);
	}

};


/*@}*/
