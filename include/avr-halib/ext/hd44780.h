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


/// Use with COutDevice
// TODO: Doku!
template <class LCDPortmap> class Hd44780
{
	public:
	void write(uint8_t data, bool wait, bool command)
	{
		UsePortmap(lcdPm, LCDPortmap);
		// alles als Output definieren
		lcdPm.rs.ddr = lcdPm.out;
		lcdPm.rw.ddr = lcdPm.out;
		lcdPm.enable.ddr = lcdPm.out;
		
		lcdPm.data.setDdr(lcdPm.out);
		
		SyncPortmap(lcdPm);
		
		lcdPm.rs.port = !command;
		lcdPm.rw.port = false;		// Schreiben
		lcdPm.enable.port = false;
		
		// Datenuebergabe jeweils 4 Bit
		lcdPm.data.setPort(data >> 4);
		
		// Enable Bit setzen und wieder loeschen
		SyncPortmap(lcdPm);
		lcdPm.enable.port = true;
		SyncPortmap(lcdPm);
		lcdPm.enable.port = false;
		SyncPortmap(lcdPm);
		// Datenuebergabe jeweils 4 Bit
		lcdPm.data.setPort(data & 0x0f);
		
		// Enable Bit setzen und wieder loeschen
		SyncPortmap(lcdPm);
		lcdPm.enable.port = true;
		SyncPortmap(lcdPm);
		lcdPm.enable.port = false;
		SyncPortmap(lcdPm);
		if (wait)
		{
			wait_busy();
		}
	}
	
	void read(uint8_t &data, bool adress)
	{
		UsePortmap(lcdPm, LCDPortmap);
		// alles als Output definieren
		lcdPm.rs.ddr = lcdPm.out;
		lcdPm.rw.ddr = lcdPm.out;
		lcdPm.enable.ddr = lcdPm.out;
		
		lcdPm.data.setDdr(lcdPm.in);
		lcdPm.data.setPort(0x00);
		
		SyncPortmap(lcdPm);
		
		lcdPm.rs.port = !adress;
		lcdPm.rw.port = true;	// lesen
		lcdPm.enable.port = false;
		
		// Enable Bit setzen und wieder loeschen
		SyncPortmap(lcdPm);
		lcdPm.enable.port = true;
		SyncPortmap(lcdPm);
		
		// Datenuebergabe jeweils 4 Bit
		data = lcdPm.data.getPin() << 4;
		
		lcdPm.enable.port = false;
		SyncPortmap(lcdPm);
		
		// Enable Bit setzen und wieder loeschen
		SyncPortmap(lcdPm);
		lcdPm.enable.port = true;
		SyncPortmap(lcdPm);
		
		// Datenuebergabe jeweils 4 Bit
		data |= lcdPm.data.getPin();
		
		lcdPm.enable.port = false;
		SyncPortmap(lcdPm);
	}
	
	void wait_busy()
	{	
		UsePortmap(lcdPm, LCDPortmap);
		// LcdHd44780 7 als Input
		lcdPm.data7.port = true;
		lcdPm.data7.ddr = lcdPm.in;
		// RW = Read
		SyncPortmap(lcdPm);
		lcdPm.rw.port = true;
		// RS auf low
		lcdPm.rs.port = false;
		// enable Bit setzen
		lcdPm.enable.port = true;
		SyncPortmap(lcdPm);
		while (lcdPm.data7.pin)
			SyncPortmap(lcdPm);
		// enable Bit wieder loeschen
		lcdPm.enable.port = false;
		
		SyncPortmap(lcdPm);
		lcdPm.enable.port = true;
		SyncPortmap(lcdPm);
		lcdPm.enable.port = false;
		SyncPortmap(lcdPm);
		// LcdHd44780 7 als Output
		lcdPm.data7.ddr = lcdPm.out;
		lcdPm.data7.port = false;
		
		lcdPm.rw.port = false;
		lcdPm.rs.port = true;
	};
	
	
	enum
	{
		display_clear = 0x01,
		home = 0x02,
		entry_mode = 0x04,
		display_mode = 0x08,
		shift_mode = 0x10,
		function_mode = 0x20,
		set_pcgram = 0x40,
		set_pddram = 0x80,
		cg_mask = 0x3f,
		dd_mask = 0x7f,
	};
	
	enum //modes
	{
		//entry_mode
		cursor_increase = 0x02,
		cursor_decrease = 0x00,
		display_shifed = 0x01,
		//display_mode
		display_on = 0x04,
		cursor_on = 0x02,
		blink_on = 0x01,
		//shift_mode
		display_shift = 0x08,
		cursor_move = 0x00,
		right_shift = 0x04,
		left_shift = 0x00,
		//function_mode
		interface_8bit = 0x10,
		interface_4bit = 0x00,
		display_2line = 0x08,
		display_1line = 0x00,
		char_5x10 = 0x04,
		char_5x7 = 0x00
	};
	
		
	Hd44780()
	{
		
		
		// power on	
		delay_ms(16);				
		// initialisierungsmuster 		 000 0010 xxxx = 32
		write(0x20, false, true);
		delay_us(5000);
		write(0x20, false, true);
		delay_us(150);
		// 4 Bit modus				 000 0010 1100 = 44 0x2c
		write((function_mode | interface_4bit | display_2line | char_5x10), false, true);	
		delay_us(5000);
		// Display off 				 000 0000 1110 = 14 0x0e
		write((display_mode | display_on | cursor_on), true, true);
		// clear display 			 000 0000 0001 = 1 0x01
		write(display_clear, true, true);
		// input mode				 000 0000 0110 = 6 0x06
		write((entry_mode | cursor_increase), true, true);
	}
	
	void setPos(uint8_t pos)
	{
		if (pos <= dd_mask )
		{
			// Sprung an Position pos
			write(set_pddram | pos, true, true);
		}
	}
	
	uint8_t getPos()
	{
		uint8_t pos;
		read(pos,true);
		return (pos );//& (dd_mask | cg_mask)) ;
	}
	
	void clear()
	{
		write(display_clear, true, true);
	}
	
	
	
};

/*@}*/
