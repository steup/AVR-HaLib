/**
 *	\file	halib/avr/uart.h
 *	\brief	Uart-Interface
 */

#pragma once

//#define HALIB_NO_VIRTUAL_DESTRUCTORS

#include "halib/avr/interrupt.h"
#include "halib/avr/regmaps.h"
#include "halib/share/cdevice.h"
#include "halib/share/queuebuffer.h"

#include <avr/io.h>




/*!	\brief UART Interface
 *	\param UartRegmap	Register map
*	\param length_t	Type used for size of the buffers and addressing the buffers
*	\param oBufLen	Size of output buffer
*	\param iBufLen	Size of input buffer
*
*/
template <class UartRegmap = Uart0, class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
	class Uart : public CDevice
{
#define rm (*(UartRegmap*)0x0)
protected:

	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;

public:

	/// Constructor
	Uart(uint32_t baudRate = 19200)
	{
		init(baudRate);
	}
	
	/// Initializes USART with given baud rate
	void init(uint32_t baudRate);
	
	/// Interrupt-Service-Routine for USART-Rx-Complete-Interrrupt. Writes received data to inBuffer.
	void onUartRecv();

	/// Interrupt-Service-Routine for USART-Data-Register-Empty-Interrrupt. Sends data from outBuffer.
	void onUartData();


	// Writes a character into the output buffer
	void putc(const char c);

	// Reads a character from the input buffer
	bool getc(char & c);
};
		
		
template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::init(uint32_t baudRate)
{
	uint8_t sreg = SREG;
	uint16_t ubrr = (((uint16_t)(CPU_FREQUENCY/16/baudRate)) - 1);
	rm.ubrrh = (uint8_t) (ubrr>>8);
	rm.ubrrl = (uint8_t) (ubrr);

	// Disable interrupts
	cli();

	// Enable UART Receiver and Transmitter, enable Receive-Interrupt
	// Data mode 8N1, asynchron
	rm.ucsrb = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
#ifndef URSEL
	rm.ucsrc = (1 << UCSZ01) | (1 << UCSZ00);
#else	
	rm.ucsrc = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
#endif	
	// Flush Receive-Buffer
	do
	{
		uint8_t dummy;
		(void) (dummy = rm.udr);
	}
	while (rm.ucsra & (1 << RXC));

	// Reset Receive and Transmit Complete-Flags
	rm.ucsra = (1 << RXC) | (1 << TXC);

	// Restore Global Interrupt-Flag
	SREG = sreg;
	
	// Set ISR for Interrupts
	//class UartRegmap::RecvInterrupt<typeof(*this), & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartData>::setInterrupt(this);
	//  
	UartRegmap::template setRecvInterrupt<Uart<UartRegmap, length_t, oBufLen, iBufLen>, & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartRecv> (*this);
	
	UartRegmap::template setDataInterrupt<Uart<UartRegmap, length_t, oBufLen, iBufLen>, & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartData> (*this);
}



template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::putc(char c)
{
	outBuffer.put(c);
	rm.ucsrb |= (1 << UDRIE); 	// enable USART-Data-Register-Empty-Interrrupt
}

template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	bool Uart<UartRegmap, length_t, oBufLen, iBufLen>::getc(char & c)
{
	return inBuffer.get(c);
}

template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartRecv()
{
	inBuffer.put(rm.udr);
}

template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartData()
{
	char c;
	if (outBuffer.get(c))
		rm.udr = c;
	else
		rm.ucsrb &= ~(1 << UDRIE); 	// disable USART-Data-Register-Empty-Interrrupt
}
#undef rm

