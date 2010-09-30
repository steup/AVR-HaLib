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

template <class LCDPortmap> class Lcm_16x4:public Hd44780<LCDPortmap>
{
public:
	
	Lcm_16x4() : pos(0){}
	
	
	void setPosi(uint8_t pos)
	{
		if (pos < 0x60)
		{
			// Sprung an Position pos
			this->pos=pos;
			Hd44780<LCDPortmap>::write(0x80 | pos, true, true);
			
		}
	}
	
	void setPos(uint8_t pos)
	{
		if(pos>=0x30)
			setPosi(0x50 + (pos - 0x30));
		else if(pos>=0x20)
			setPosi(0x10 + (pos - 0x20));
		else if(pos>=0x10)
			setPosi(0x40 + (pos - 0x10));
		else //if(pos>=0x00)
			setPosi(0x00 + (pos - 0x00));
		
	}
	
	
	uint8_t getPos()
	{
		if(pos >= 0x50)
			return pos - 0x50 + 0x30;
		if(pos >= 0x40)
			return pos - 0x40 + 0x10;
		if(pos >= 0x10)
			return pos - 0x10 + 0x20;
		return pos - 0x00;
	}
	
	uint8_t getPosi()
	{
// 		return pos;
		return Hd44780<LCDPortmap>::getPos();
	}
	void clear()
	{
		pos=0;
		Hd44780<LCDPortmap>::clear();
	}
	/// Writes a character to Display
	void put(const char c)
	{
		if(c=='\n')
		{
			if(getPos()>=0x30)
			{
				setPos(0x00);
			}else if(getPos()>=0x20)
			{
				setPos(0x30);
			}else if(getPos()>=0x10)
			{
				setPos(0x20);
			}else /*if(getPos()>=0x00)*/
			{
				setPos(0x10);
			}
		}
		else
		{
			Hd44780<LCDPortmap>::write(c, true, false);
			pos++;
		}
	}
	private:
		uint8_t pos;
};

template <class LCDPortmap> class LcdHd44780:public Lcm_16x4<LCDPortmap>{};


/*@}*/
