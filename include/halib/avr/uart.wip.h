/**
 *	\file	halib/uart.h
 *	\brief	Definiert und Implementiert die Uart-Schnittstelle des at90can128.
 */

#pragma once

#include "halib/avr/interrupt.h"
#include <avr/io.h>


/// Abstrakte Interrupt-Klasse f&uuml;r den USART-Rx-Complete-Interrrupt.
UseInterrupt(SIG_UART0_RECV);
/// Abstrakte Interrupt-Klasse f&uuml;r den USART-Data-Register-Empty-Interrrupt.
UseInterrupt(SIG_UART0_DATA);

// Problem: Welcher Interupt ist zu binden? Lösung?: Binden der Interupts durch den Benutzer
// Idee: Standard: alle; Optimierung: abschalten/don't-use

// Register map
struct Uart0
{
	volatile uint8_t :?? ;
	volatile uint8_t ubrrh;
	volatile uint8_t :?? ;
	volatile uint8_t ubrrl;
	volatile uint8_t :?? ;
	volatile uint8_t ucsra;
	volatile uint8_t :?? ;
	volatile uint8_t ucsrb;
	volatile uint8_t :?? ;
	volatile uint8_t ucsrc;
	volatile uint8_t :?? ;
	volatile uint8_t udr;
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<typename T, void (T::*Fxn)() const>
	static void setRecvInterrupt(T const * obj)
	{
		redirectISRMF(SIG_UART0_RECV, Fxn, obj);
	};
	template<typename T, void (T::*Fxn)() const>
	static void setDataInterrupt(T const * obj)
	{
		redirectISRMF(SIG_UART0_DATA, Fxn, obj);
	};
};

/*!	\brief UART Interface
 *	\param UartRegmap	Register map
*	\param length_t	Typ für die Indexierung / Gr&ouml;&szlig;e der Puffer
*	\param oBufLen	Gr&ouml;&szlig;e des Ausgangspuffers
*	\param iBufLen	Gr&ouml;&szlig;e des Eingangspuffers
*
*/
template <class UartRegmap = Uart0, class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
		class Uart : public CDevice
{
protected:

	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;

public:

	//UDR0/ Constructor
	Uart(uint32_t baudRate = 19200)
	{
		init(baudRate);
	}
	
	void init(uint32_t baudRate);
	
	/// Interrupt-Service-Routine für USART-Rx-Complete-Interrrupt. Schreibt emfangene Daten in inBuffer.
	void onUartRecv();

	/// Interrupt-Service-Routine für USART-Data-Register-Empty-Interrrupt. Sendet Daten aus outBuffer.
	void onUartData();


	void putc(const char);
	char getc();
};
		
		
template <class length_t, length_t oBufLen, length_t iBufLen>
		Uart<length_t,oBufLen,iBufLen>::init(uint32_t baudRate)
{
	uint8_t sreg = SREG;
	uint16_t ubrr = (((uint16_t)(CPU_FREQUENCY/baudRate/16)) - 1);
	rm.ubrrh = (uint8_t) (ubrr>>8);
	rm.ubrrl = (uint8_t) (ubrr);

	// Interrupts kurz deaktivieren
	cli();

	// UART Receiver und Transmitter anschalten, Receive-Interrupt aktivieren
	// Data mode 8N1, asynchron
	rm.ucsrb = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
#ifndef URSEL
	rm.ucsrc = (1 << UCSZ01) | (1 << UCSZ00);
#else	
	rm.ucsrc = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
#endif	
	// Flush Receive-Buffer (entfernen evtl. vorhandener ungltiger Werte)
	do
	{
		uint8_t dummy;
		(void) (dummy = UDR0);
	}
	while (UCSR0A & (1 << RXC));

	// Rcksetzen von Receive und Transmit Complete-Flags
	rm.ucsra = (1 << RXC) | (1 << TXC);

	// Global Interrupt-Flag wieder herstellen
	SREG = sreg;
	
	rm.setDataInterrupt<typeof(*this), & Uart<length_t,oBufLen,iBufLen>::onUartData>(this);
	rm.setRecvInterrupt<typeof(*this), & Uart<length_t,oBufLen,iBufLen>::onUartRecv>(this);
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
		void Uart<length_t,oBufLen,iBufLen>::onUartRecv()
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


