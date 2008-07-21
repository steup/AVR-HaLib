
#include "avr-halib/avr/portmap.h"
#include "avr-halib/share/delay.h"


/// Use with Coutdevice
	
template <class LCDPortmap> class LcdHd44780
{
	void write(char data, bool wait, bool command)
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
		
		// LcdHd44780 7 als Output
		lcdPm.data7.ddr = lcdPm.out;
		lcdPm.data7.port = false;
		
		lcdPm.rw.port = false;
		lcdPm.rs.port = true;
	};
public:
	
	LcdHd44780()
	{
		
		
		// power on	
		delay_ms(16);				
		// initialisierungsmuster 		 000 0010 xxxx = 32
		write(0x20, false, true);
		delay_us(5000);
		write(0x20, false, true);
		delay_us(150);
		// 4 Bit modus				 000 0010 1100 = 44
		write(0x2c, false, true);	
		delay_us(5000);
		// Display off 				 000 0000 1110 = 14
		write(0x0e, true, true);
		// clear display 			 000 0000 0001 = 1
		write(0x01, true, true);
		// input mode				 000 0000 0110 = 6		
		write(0x06, true, true);
	}
	
	void setPos(uint8_t pos)
	{
		if (pos < 0x60)
		{
			// Sprung an Position pos
			write(0x80 | pos, true, true);
		}
	}
	/// Writes a character to Display
	void put(const char c)
	{
		write(c, true, false);
	}

};
/*
template <class LCDPortmap> class LcdHd44780tabbed: public LcdHd44780< LCDPortmap>
{
public:
	void put(const char c)
	{
		if( c == '\t')
		{
			if 
		}
		else if (c == '\n')
		{}
		else
		write(c, true, false);
	}
};
*/
