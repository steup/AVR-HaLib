/** \addtogroup avr */
/*@{*/
/**
 *	\file	avr-halib/avr/uart.h
 *	\brief	Defines Uart
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once


#include "avr-halib/avr/interrupt.h"
#include "avr-halib/avr/regmaps.h"
#include "avr-halib/share/queuebuffer.h"


/*!
 *	\class Uart uart.h "avr-halib/avr/uart.h"
 *	\brief UART Interface
 *	\param UartRegmap	Register map
 *	\param length_t	Type used for size of the buffers and addressing the buffers
 *	\param oBufLen	Size of output buffer
 *	\param iBufLen	Size of input buffer
 *
 *	For reading and writing strings and integers see \see doc_cdevices
 */
template <class UartRegmap = struct Uart0<>, class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
	class Uart
{
protected:
	typedef class UartRegmap::Controller_Configuration Controller_Configuration;
	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;


	inline void configure()__attribute__ ((always_inline))
	{
		UseRegmap(rm, UartRegmap);
		// Data mode 8N1, asynchron
		rm.template configure<8,'N',1>();
		
		// Enable UART Receiver and Transmitter, enable Receive-Interrupt
		rm.rxen = true;
		rm.txen = true;
		
		rm.rxcie = true;
		rm.txcie = false;
		rm.udrie = false;
		
		rm.ucsz2 = false; //also set by rm.configure
		
		rm.rxb8 = false;
		rm.txb8 = false;
		
		// Flush Receive-Buffer
		do
		{
			uint8_t dummy;
			(void) (dummy = rm.udr);
			SyncRegmap(rm);
		}
		while (rm.rxc);
	
		// Reset Receive and Transmit Complete-Flags
		rm.rxc = false;
		rm.txc = true;
		rm.u2x = true;
		rm.mpcm = false;
		
		//write errorflags false
		rm.fe = false;
		rm.pe = false;
		rm.dor = false;
		rm.udre = false;
	}
public:
	enum
	{	
		DoubleSpeedBaudRateRegister = ((Controller_Configuration::controllerClk/4/UartRegmap::baudrate)-1)/2,
		BaudRateRegister = ((Controller_Configuration::controllerClk/8/UartRegmap::baudrate)-1)/2
	};

	/// Constructor
	Uart(uint32_t baudRate/* = UartRegmap::baudrate*/)
	{
		init(baudRate);
	}
	Uart()
	{
		init();
	}
	
	/// Initializes USART with given baud rate
	void init(uint32_t baudRate)
	{
		UseRegmap(rm, UartRegmap);
		
		rm.setbaudrateU2X(baudRate);
		
		configure();
	
		SyncRegmap(rm);
		// Set ISR for Interrupts
		//  
		UartRegmap::template setRecvInterrupt<Uart<UartRegmap, length_t, oBufLen, iBufLen>, & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartRecv> (*this);
		
		UartRegmap::template setDataInterrupt<Uart<UartRegmap, length_t, oBufLen, iBufLen>, & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartData> (*this);
	}
	
	void init()
	{
		UseRegmap(rm, UartRegmap);
		
		rm.setbaudrateU2X(UartRegmap::baudrate);
		
		configure();
		
		SyncRegmap(rm);
		// Set ISR for Interrupts
		//  
		UartRegmap::template setRecvInterrupt<Uart<UartRegmap, length_t, oBufLen, iBufLen>, & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartRecv> (*this);
		
		UartRegmap::template setDataInterrupt<Uart<UartRegmap, length_t, oBufLen, iBufLen>, & Uart<UartRegmap, length_t, oBufLen, iBufLen>::onUartData> (*this);
	}
	
	
	/// Interrupt-Service-Routine for USART-Rx-Complete-Interrrupt. Writes received data to inBuffer.
	void onUartRecv()
	{
		UseRegmap(rm, UartRegmap);
		
		inBuffer.put(rm.udr);
	}

	/// Interrupt-Service-Routine for USART-Data-Register-Empty-Interrrupt. Sends data from outBuffer.
	void onUartData()
	{
		UseRegmap(rm, UartRegmap);
		
		char c;
		if (outBuffer.get(c))
			rm.udr = c;
		else
			rm.udrie=false; 	// disable USART-Data-Register-Empty-Interrrupt
		SyncRegmap(rm);
	}


	/// Writes a character into the output buffer
	void put(const char c)
	{
		UseRegmap(rm, UartRegmap);
	
		outBuffer.put(c);
		rm.udrie=true;	// enable USART-Data-Register-Empty-Interrrupt 
		SyncRegmap(rm);
	}

	/**	\brief	Reads a character from the input buffer
	 *	\param	c	Reference to variable which shall store the character
	 *	\return		true if a character was read
	 */
	bool get(char & c)
	{
		return inBuffer.get(c);
	}


};

/*!	\brief UART Interface
 *	\param UartRegmap	Register map
 *	\param length_t	Type used for size of the buffers and addressing the buffers
 *	\param oBufLen	Size of output buffer
 *	\param iBufLen	Size of input buffer
 *
 *	For reading and writing strings and integers see \see doc_cdevices
 */
template <class UartRegmap = struct Uart0<> >
	class Uartnoint
{
protected:
	typedef class UartRegmap::Controller_Configuration Controller_Configuration;
		
	inline void configure(UartRegmap rm){
	
		// Data mode 8N1, asynchron
		rm.template configure<8,'N',1>();
		
		// Enable UART Receiver and Transmitter, enable Receive-Interrupt
		rm.rxen = true;
		rm.txen = true;
		
		rm.rxcie = false;
		rm.txcie = false;
		rm.udrie = false;
		
		rm.ucsz2 = false; //also set by rm.configure
		
		rm.rxb8 = false;
		rm.txb8 = false;
		
		// Flush Receive-Buffer
		do
		{
			uint8_t dummy;
			(void) (dummy = rm.udr);
			SyncRegmap(rm);
		}
		while (rm.rxc);
	
		// Reset Receive and Transmit Complete-Flags
		rm.rxc = false;
		rm.txc = true;
		rm.u2x = true;//false;
		rm.mpcm = false;
		
		//write errorflags false
		rm.fe = false;
		rm.pe = false;
		rm.dor = false;
		rm.udre = false;

	
	}

public:
// 	enum
// 	{	
// 		DoubleSpeedBaudRateRegister = ((Controller_Configuration::controllerClk/4/UartRegmap::baudRate)-1)/2,
// 		BaudRateRegister = ((Controller_Configuration::controllerClk/8/UartRegmap::baudRate)-1)/2
// 	};

	/// Constructor
	Uartnoint()
	{
		init();
	}
	
	Uartnoint(uint32_t baudRate = UartRegmap::baudrate)
	{
		init(UartRegmap::baudrate);
	}
	
	/// Initializes USART with given baud rate
	void init(uint32_t baudRate)
	{
		UseRegmap(rm, UartRegmap);
		
		rm.setbaudrateU2X(baudRate);
	
		// Data mode 8N1, asynchron
		rm.template configure<8,'N',1>();
		
		configure(rm);
		
		SyncRegmap(rm);
	}
	
	void init()
	{
		UseRegmap(rm, UartRegmap);
		
// 		rm.setbaudrateU2X<UartRegmap::baudrate>();
	
		// Data mode 8N1, asynchron
		rm.template configure<8,'N',1>();
		
		configure(rm);
		
		SyncRegmap(rm);
	}
	
	
	/// Writes a Char to the uart
	void put(const char c)
	{
		UseRegmap(rm, UartRegmap);
		while(!rm.udre)SyncRegmap(rm);
		rm.udr = c;
		SyncRegmap(rm);
	}

	/**	\brief	Reads a character from the udr buffer
	 *	\param	c	Reference to variable which shall store the character
	 *	\return		true if a character was read
	 */
	bool get(char & c)
	{
		UseRegmap(rm, UartRegmap);
		SyncRegmap(rm);
		bool ret = rm.rxc;
		c=rm.udr;
		return ret;
	}
};

/*@}*/
