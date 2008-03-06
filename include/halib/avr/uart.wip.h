/**
 *	\file	halib/avr/uart.h
 *	\brief	Uart-Interface
 *	\todo	Support of sending and receiving null characters
 */

#pragma once

#include "halib/share/cdevice.h"
#include "halib/share/queuebuffer.h"
#include "halib/avr/interrupt.h"

#include <avr/io.h>


GenInterrupt(SIG_UART0_RECV);
GenInterrupt(SIG_UART0_DATA);

GenInterrupt(SIG_UART1_RECV);
GenInterrupt(SIG_UART1_DATA);

// Problem: Welcher Interupt ist zu binden? Lösung?: Binden der Interupts durch den Benutzer
// Idee: Standard: alle; Optimierung: abschalten/don't-use

// Register map
struct Uart0
{
	volatile uint8_t : 0xc0 * 8;
	volatile uint8_t ucsra;
	volatile uint8_t ucsrb;
	volatile uint8_t ucsrc;
	volatile uint8_t : 8;
	volatile uint8_t ubrrl;
	volatile uint8_t ubrrh;
	volatile uint8_t udr;
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
	static void setRecvInterrupt(T & obj)
	{
		redirectISRMF(SIG_UART0_RECV, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setDataInterrupt(T & obj)
	{
		redirectISRMF(SIG_UART0_DATA, Fxn, obj);
	}
};

struct Uart1
{
	volatile uint8_t : 0xc8 * 8;
	volatile uint8_t ucsra;
	volatile uint8_t ucsrb;
	volatile uint8_t ucsrc;
	volatile uint8_t : 8;
	volatile uint8_t ubrrl;
	volatile uint8_t ubrrh;
	volatile uint8_t udr;
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
	static void setRecvInterrupt(T & obj)
	{
		redirectISRMF(SIG_UART1_RECV, Fxn, obj);
	}
	template<class T, void (T::*Fxn)()>
	static void setDataInterrupt(T & obj)
	{
		redirectISRMF(SIG_UART1_DATA, Fxn, obj);
	}
};


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


	void putc(const char);
	char getc();
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
//	rm.setRecvInterrupt<typeof(*this), & onUartRecv>(this);
}



// Writes a character into the output buffer
template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::putc(char out)
{
	DDRA |= 2;
	PORTA = (PORTA & 2) ^ 2 | PORTA & ~2;
	outBuffer.put(out);
	rm.ucsrb |= (1 << UDRIE); 	// enable USART-Data-Register-Empty-Interrrupt
}

// Reads a character from the input buffer
template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	char Uart<UartRegmap, length_t, oBufLen, iBufLen>::getc()
{
	DDRA |= 8;
	PORTA = (PORTA & 8) ^ 8 | PORTA & ~8;
	return inBuffer.get();
}

template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartRecv()
{
	DDRA |= 4;
	PORTA = (PORTA & 4) ^ 4 | PORTA & ~4;
	inBuffer.put(rm.udr);
}

template <class UartRegmap, class length_t, length_t oBufLen, length_t iBufLen>
	void Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartData()
{
	DDRA |= 1;
	PORTA = (PORTA & 1) ^ 1 | PORTA & ~1;
	uint8_t c = outBuffer.get();
	if (c > 0)
		rm.udr = c;
	else
		rm.ucsrb &= ~(1 << UDRIE); 	// disable USART-Data-Register-Empty-Interrrupt
}
#undef rm

