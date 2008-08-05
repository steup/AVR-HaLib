/**
 *	\file	avr-halib/avr/uart.h
 *	\brief	Uart-Interface
 */

#pragma once


#include "avr-halib/avr/interrupt.h"
#include "avr-halib/avr/regmaps.h"
#include "avr-halib/share/queuebuffer.h"

#include <avr/io.h>

class DefineController
{
	public:
		enum
	{
		controllerClk=CPU_FREQUENCY
	};

};
#if 0
class Uart0w
{
private:
	uint8_t __base [0xc0];
public:
	union{
		uint8_t ucsra;
		struct{
			bool	mpcm:1;
			bool	u2x:1;
			bool	upe:1;
			bool	dor:1;
			bool	fe:1;
			bool	udre:1;
			bool	txc:1;
			bool	rxc:1;
		};
	};
	union{
		uint8_t ucsrb;
		struct{
			bool	txb8:1;
			bool	rxb8:1;
			bool	ucsz2:1;
			bool	txen:1;
			bool	rxen:1;
			bool	udrie:1;
			bool	txcie:1;
			bool	rxcie:1;
		};
	};
	union{
		uint8_t ucsrc;
		struct ucsrc{
			bool	ucpol:1;
			bool	ucsz0:1;
			bool	ucsz1:1;
			bool	usbs:1;
			uint8_t	upm:2;
			bool	umsel:1;
			bool	:1;//reserved
		};
	};
private:
	uint8_t __pad0;
public:
	union{	
		struct{
			uint8_t ubrrl;
			uint8_t ubrrh;
		};
		uint16_t ubbr;
	};
	uint8_t udr;
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
	static void setRecvInterrupt(T & obj)
	{
		redirectISRM(SIG_UART0_RECV, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setDataInterrupt(T & obj)
	{
		redirectISRM(SIG_UART0_DATA, Fxn, obj);
	}
};
#endif
template <class pController_Configuration = DefineController> class Uart1w
{
private:
	uint8_t __base [0xc8];
public:
	
	union{
		uint8_t ucsra;
		struct{
			bool	mpcm:1;
			bool	u2x:1;
			bool	pe:1;
			bool	dor:1;
			bool	fe:1;
			bool	udre:1;
			bool	txc:1;
			bool	rxc:1;
		};
	};
	union{
		uint8_t ucsrb;
		struct{
			bool	txb8:1;
			bool	rxb8:1;
			bool	ucsz2:1;
			bool	txen:1;
			bool	rxen:1;
			bool	udrie:1;
			bool	txcie:1;
			bool	rxcie:1;
		};
	};
	union{
		uint8_t ucsrc;
		struct {
			bool	ucpol:1;
			bool	ucsz0:1;
			bool	ucsz1:1;
			bool	usbs:1;
			uint8_t	upm:2;
			bool	umsel:1;
			bool	:1; //reserved
		};
	};
	union Ucsrc{	
			uint8_t data;
			struct {bool	ucpol:1;
			bool	ucsz0:1;
			bool	ucsz1:1;
			bool	usbs:1;
			uint8_t	upm:2;
			bool	umsel:1;
			bool	:1; //reserved;
			};
		};
private:
	uint8_t __pad0;
public:
	union{	
		struct{
			uint8_t ubrrl;
			uint8_t ubrrh;
		};
		uint16_t ubbr;
	};
		
	uint8_t udr;
	enum{noParity=0x00,evenParity=0x2,oddParity=0x3};
	enum{baudRate=19200};
	typedef pController_Configuration Controller_Configuration;
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
	static void setRecvInterrupt(T & obj)
	{
		redirectISRM(SIG_UART1_RECV, Fxn, obj);
	}
	template<class T, void (T::*Fxn)()>
	static void setDataInterrupt(T & obj)
	{
		redirectISRM(SIG_UART1_DATA, Fxn, obj);
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
template <class UartRegmap = Uart0, class length_t = uint8_t, length_t oBufLen = 255, length_t iBufLen = 20>
	class Uart
{
protected:
	typedef class UartRegmap::Controller_Configuration Controller_Configuration;
	QueueBuffer<char, length_t, iBufLen> inBuffer;
	QueueBuffer<char, length_t, oBufLen> outBuffer;
	
public:
	enum
	{	
		DoubleSpeedBaudRateRegister = (Controller_Configuration::controllerClk/8/UartRegmap::baudRate)-1,
		BaudRateRegister = (Controller_Configuration::controllerClk/16/UartRegmap::baudRate)-1
	};

	/// Constructor
	Uart(uint32_t baudRate = UartRegmap::baudRate)
	{
		init(baudRate);
	}
	
	/// Initializes USART with given baud rate
	void init(uint32_t baudRate)
	{
		UseRegmap(rm, UartRegmap);
		
		uint16_t ubrr = (((uint16_t)(Controller_Configuration::controllerClk/16/baudRate)) - 1);
		rm.ubrrh = (uint8_t) (ubrr>>8);
		rm.ubrrl = (uint8_t) (ubrr);
	
		// Disable interrupts
		//cli();
	
		// Enable UART Receiver and Transmitter, enable Receive-Interrupt
		// Data mode 8N1, asynchron
		rm.rxen = true;
		rm.txen = true;
		rm.rxcie = true;
		rm.txcie = false;
		rm.udrie = false;
		rm.ucsz2 = false;
		rm.rxb8 = false;
		rm.txb8 = false;
		
		struct UartRegmap::Ucsrc ucsrc;
		ucsrc.data = 0;
		#ifdef URSEL
		ucsrc.ursel = true;
		#endif	
		
		ucsrc.ucsz1 = true;
		ucsrc.ucsz0 = true;
		ucsrc.usbs = false;
		ucsrc.upm = UartRegmap::noParity;
		rm.ucsrc = ucsrc.data;
		
		// Flush Receive-Buffer
		do
		{
			uint8_t dummy;
			(void) (dummy = rm.udr);
			SyncRegmap(rm);
		}
		while (rm.rxc);
	
		// Reset Receive and Transmit Complete-Flags
		rm.rxc = true;
		rm.txc = true;
		rm.u2x = false;
		rm.mpcm = false;
		
		rm.fe = false;
		rm.pe = false;
		rm.dor = false;
		rm.udre = false;
		// Restore Global Interrupt-Flag
// 		SREG = sreg;
		
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


