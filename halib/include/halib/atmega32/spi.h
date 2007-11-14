/**
 *	\file	halib/atmega32/spi.h
 *	\brief	Definiert und Implementiert die SPI-Schnittstelle des ATmega32.
 */

#pragma once

#include "halib/atmega23/interrupt.h"

#include <avr/io.h>
#include <avr/interrupt.h>


DECLARE_INTERRUPT_CLASS(InterruptSpi);		// Serial Transfer Complete

/*!	\brief SPI-Schnittstelle des ATmega32
 * 
 * 	Definiert und Implementiert die SPI-Schnittstelle des ATmega32 f&uuml;r slave betrieb 
 * 	(Bibliothek ist ungetestet)
 *
 *	\param length_t	Typ f&uuml;r die Indexierung / Gr&ouml;&szlig;e der Puffer
 *	\param oBufLen	Gr&ouml;&szlig;e des Ausgangspuffers
 *	\param iBufLen	Gr&ouml;&szlig;e des Eingangspuffers
 *
 */
template <class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
		class Spi : public CDevice, public InterruptSpi
{
	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;
	
public:
		
	Spi();

	
	/// Interrupt-Service-Routine f&uuml;r SPI-Interrrupt. Schreibt emfangene Daten in inBuffer und sendet Daten aus outBuffer.
	
	void onInterruptSpi();

	/**
	 * putc &uuml;bernimmt ein Zeichen als Parameter und schreibt dieses in den Ausgangspufferspeicher der SPI-Schnittstelle.
	 */
	void putc (const char);

	/**
	 * getc gibt wenn noch vorhanden ein Zeichen aus dem Empfangs Buffer aus sonst 0 NUL
	 */ 
	char getc();
};


template <class length_t, length_t oBufLen, length_t iBufLen>
		Spi<length_t,oBufLen,iBufLen>::Spi()
{

	uint8_t sreg = SREG;
	cli(); // Interrupts kurz deaktivieren
	
	SPCR = (1 << SPIE)|(1 << SPE)|(0 << DORD)|(0 << MSTR)|(0 << CPOL)|(0 << CPHA)|(0 << SPR1)|(0 << SPR0);
	//SPI aktivieren, Intrupt aktivieren, Slave Mode

	// Global Interrupt-Flag wieder herstellen
	SREG = sreg;

	// enable interrupts
	sei();
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		void Spi<length_t,oBufLen,iBufLen>::putc(char out)
{
	outBuffer.put(out);
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		char Spi<length_t,oBufLen,iBufLen>::getc()
{
	return inBuffer.get();
}

template <class length_t, length_t oBufLen, length_t iBufLen>
		void Spi<length_t,oBufLen,iBufLen>::onInterruptSpi()
{
	inBuffer.put(SPDR);
	SPDR = outBuffer.get();
}
