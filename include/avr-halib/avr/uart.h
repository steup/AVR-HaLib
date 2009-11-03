/** \addtogroup avr */
/*@{*/
/**
 *	\file	avr-halib/avr/uart.h
 *	\brief	Defines Uart
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/share/delegate.h"
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
template <class UartRegmap = struct Uart1<> >class Uartnoint
{
protected:
	typedef class UartRegmap::Controller_Configuration Controller_Configuration;
	
	inline void configure()__attribute__ ((always_inline))
	{
		UseRegmap(rm, UartRegmap);
		
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
		rm.txc = false;
		rm.u2x = true;
		rm.mpcm = false;
		
		//write errorflags false
		rm.fe = false;
		rm.pe = false;
		rm.dor = false;
		rm.udre = false;
	}
public:
	typedef	class UartRegmap::RecvInterrupt RecvInterrupt;
	typedef	class UartRegmap::DataInterrupt DataInterrupt;
// 	RecvInterrupt onReceive;
// 	DataInterrupt onReady;

	/// Constructor
	Uartnoint(uint32_t baudRate)
	{
		init(baudRate);
	}
	
	Uartnoint()
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
	}
	
	/// Initializes USART with given baud rate
	void init()
	{
		UseRegmap(rm, UartRegmap);
		
		rm.setbaudrateU2X(UartRegmap::baudrate);
		
		configure();
			
		SyncRegmap(rm);
	}
	
	void enableonReceive()
	{	
		UseRegmap(rm, UartRegmap);
		rm.rxcie=true;
		SyncRegmap(rm);
	}

	void enableonReady()
	{
		UseRegmap(rm, UartRegmap);
		rm.udrie=true;
		SyncRegmap(rm);
	}
	void disableonReceive()
	{	
		UseRegmap(rm, UartRegmap);
		rm.rxcie=false;
		SyncRegmap(rm);
	}

	void disableonReady()
	{
		UseRegmap(rm, UartRegmap);
		rm.udrie=false;
		SyncRegmap(rm);
	}

	/// Writes a character into the output buffer
	void put(const char c)
	{
		UseRegmap(rm, UartRegmap);
		rm.udr = c;
		SyncRegmap(rm);
	}

	bool ready()__attribute__ ((always_inline))
	{
		UseRegmap(rm, UartRegmap);
		SyncRegmap(rm);
		return rm.udre;
	}
	
	/**	\brief	Reads a character from the input buffer
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

template <class UartRegmap = struct Uart1<> >class Uart:
	public Uartnoint<UartRegmap>
{
public:
	typedef	class UartRegmap::RecvInterrupt RecvInterrupt;
	typedef	class UartRegmap::DataInterrupt DataInterrupt;
	RecvInterrupt onReceive;
	DataInterrupt onReady;
	/// Constructor
	Uart(uint32_t baudRate):Uartnoint<UartRegmap>(baudRate)
	{}
	
	Uart():Uartnoint<UartRegmap>()
	{}
};