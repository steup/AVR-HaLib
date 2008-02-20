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


/*!	\brief UART-Schnittstelle des ATmega32
*	\param length_t	Typ für die Indexierung / Gr&ouml;&szlig;e der Puffer
*	\param oBufLen	Gr&ouml;&szlig;e des Ausgangspuffers
*	\param iBufLen	Gr&ouml;&szlig;e des Eingangspuffers
*
*/
template <class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
		class Uart : public CDevice
{
protected:

	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;

public:

	/// Konstruktor
	Uart();
	
	/// Interrupt-Service-Routine für USART-Rx-Complete-Interrrupt. Schreibt emfangene Daten in inBuffer.
	void onInterruptUartRecv();

	/// Interrupt-Service-Routine für USART-Data-Register-Empty-Interrrupt. Sendet Daten aus outBuffer.
	void onInterruptUartData();


	void putc(const char);
	char getc();
};


template <class length_t, length_t oBufLen, length_t iBufLen>
		Uart<length_t,oBufLen,iBufLen>::Uart()
{
	uint8_t sreg = SREG;
	uint16_t ubrr = ((CPU_FREQUENCY/UART_BAUDRATE/16) - 1);
	UBRR0H = (uint8_t) (ubrr>>8);
	UBRR0L = (uint8_t) (ubrr);

	// Interrupts kurz deaktivieren
	cli();

	// UART Receiver und Transmitter anschalten, Receive-Interrupt aktivieren
	// Data mode 8N1, asynchron
	UCSR0B = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
#ifndef URSEL
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
#else	
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
#endif	
	// Flush Receive-Buffer (entfernen evtl. vorhandener ungltiger Werte)
	do
	{
		uint8_t dummy;
		(void) (dummy = UDR0);
	}
	while (UCSR0A & (1 << RXC));

	// Rcksetzen von Receive und Transmit Complete-Flags
	UCSR0A = (1 << RXC) | (1 << TXC);

	// Global Interrupt-Flag wieder herstellen
	SREG = sreg;
	
	redirectISRMF(InterruptUartRecv,)
	redirectISRMF(InterruptUartRecv,)
			IMPLEMENT_INTERRUPT_CLASS(SIG_UART0_RECV, InterruptUartRecv)
			IMPLEMENT_INTERRUPT_CLASS(SIG_UART0_DATA, InterruptUartData)
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


