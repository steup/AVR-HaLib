/**
 *	\file	halib/at90can128/uart.h
 *	\brief	Definiert und Implementiert die Uart-Schnittstelle des at90can128.
 */

#pragma once

#include "halib/at90can128/interrupt.h"

#include <avr/io.h>


/*!	\brief UART-Schnittstelle des ATmega32
*	\param length_t	Typ f�r die Indexierung / Gr&ouml;&szlig;e der Puffer
*	\param oBufLen	Gr&ouml;&szlig;e des Ausgangspuffers
*	\param iBufLen	Gr&ouml;&szlig;e des Eingangspuffers
*
*/
template <class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
		class Lcd : public CDevice
{
private:
	write

protected:

	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;

public:

	/// Konstruktor
	Lcd();
	
	
	void putc(const char);
	char getc();
	void flush();
	void command(const char);
};


template <class length_t, length_t oBufLen, length_t iBufLen>
		Lcd<length_t,oBufLen,iBufLen>::Lcd()
{
	void lcd_init() 
{ 
	//power on	
	wait(15000);				
	// initialisierungsmuster 		 000 0011 xxxx = 48
	// noch im 8 bit Modus
	lcd_write(48,4100,0);			
	lcd_write(48,100,0);
	lcd_write(48,0,0);
	// Wechsel auf 4 Bit modus		 000 0010 xxxx = 32
	lcd_write(32,0,0);	
	// Aufloesung, Zeilenzahl		 000 0010 10xx = 40
	// Matrix 5x8, 2 Zeilen
	lcd_write(40,0,0);
	// Display on				 000 0000 1110 = 14
	// Cursor an, Blinken aus
	// Fehler im Datenblatt !!!
	lcd_write(14,0,0);
	// clear display 			 000 0000 0001 = 1
	lcd_write(1,0,0);						
	// input mode				 000 0000 0110 = 6
	// inkrementierender Cursor		
	lcd_write(6,0,0);
	return; 
}
}

void template <class length_t, length_t oBufLen, length_t iBufLen>
		Lcd<length_t,oBufLen,iBufLen>::write(char data, int time, bool type)
{
	// alles als Output definieren
	RS_DDR |=(1<<RS_BIT);		
	RW_DDR |=(1<<RW_BIT);
	E_DDR |=(1<<E_BIT);		
	LCD_4_DDR |=(1<<LCD_4_BIT);
	LCD_5_DDR |=(1<<LCD_5_BIT);	
	LCD_6_DDR |=(1<<LCD_6_BIT);
	LCD_7_DDR |=(1<<LCD_7_BIT);

	// alles low
	RS_PORT &=~(1<<RS_BIT);		// Steuerregister	
	RW_PORT &=~(1<<RW_BIT);		// Schreiben
	E_PORT &=~(1<<E_BIT);	
	LCD_7_PORT &=~(1<<LCD_7_BIT);
	LCD_6_PORT &=~(1<<LCD_6_BIT);	
	LCD_5_PORT &=~(1<<LCD_5_BIT);
	LCD_4_PORT &=~(1<<LCD_4_BIT);		
			
	if (type!=0)
	{
		RS_PORT |= (1<<RS_BIT);	  // Datenregister
	}

	// Datenuebergabe jeweils 4 Bit
	for (int n=7;n>=0;n--)
	{
		if (data & (1<<n))
		{
			switch (n)
			{
				case 7: LCD_7_PORT |= (1<<LCD_7_BIT);break;
				case 6: LCD_6_PORT |= (1<<LCD_6_BIT);break;
				case 5:	LCD_5_PORT |= (1<<LCD_5_BIT);break;
				case 4:	LCD_4_PORT |= (1<<LCD_4_BIT);break;
				case 3:	LCD_7_PORT |= (1<<LCD_7_BIT);break;
				case 2:	LCD_6_PORT |= (1<<LCD_6_BIT);break;
				case 1:	LCD_5_PORT |= (1<<LCD_5_BIT);break;
				case 0:	LCD_4_PORT |= (1<<LCD_4_BIT);break;
			}
		}
		if ((n==4) || (n==0))
		{
			// enable Bit setzen und wieder loeschen
			E_PORT|=(1<<E_BIT);
			E_PORT&=~(1<<E_BIT);
			// Datenbits auf low
			LCD_7_PORT &=~(1<<LCD_7_BIT);
			LCD_6_PORT &=~(1<<LCD_6_BIT);	
			LCD_5_PORT &=~(1<<LCD_5_BIT);
			LCD_4_PORT &=~(1<<LCD_4_BIT);
		}
	}	
	if (time==0)
	{
		wait_LCDbusy();
	}
	else
	{
		wait(time);
	}
}

 

void wait(unsigned int t)
{
	do
	{
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}while(--t);
}

void wait_LCDbusy()
{	
	// LCD 7 als Input
	LCD_7_PORT |=(1<<LCD_7_BIT);
	LCD_7_DDR &=~(1<<LCD_7_BIT);
	// RW = Read
	RW_PORT |=(1<<RW_BIT);
	// enable Bit setzen
	E_PORT|=(1<<E_BIT);
	while (LCD_7_PIN & (1<<LCD_7_BIT));
	// enable Bit wieder loeschen
	E_PORT&=~(1<<E_BIT);
	// LCD 7 als Output
	LCD_7_DDR |=(1<<LCD_7_BIT);
	LCD_7_PORT &=~(1<<LCD_7_BIT);
	RW_PORT &=~(1<<RW_BIT);			
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		void Uart<length_t,oBufLen,iBufLen>::putc(char out)
{
	outBuffer.put(out);
	UCSR0B |= (1 << UDRIE); 	// aktiviere interupt fuer leeren uart buffer
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		char Uart<length_t,oBufLen,iBufLen>::getc()
{
	return inBuffer.get();
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		void Uart<length_t,oBufLen,iBufLen>::onInterruptUartRecv()
{
	// Auf Pointer-Ueberprfung aus Laufzeitgrnden verzichtet!
	inBuffer.put(UDR0);
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		void Uart<length_t,oBufLen,iBufLen>::onInterruptUartData()
{
	// Auf Pointer-Ueberprfung aus Laufzeitgrnden verzichtet!
	uint8_t c = outBuffer.get();
	if (c > 0)
		UDR0 = c;
	else
		UCSR0B &= ~(1 << UDRIE);
}


