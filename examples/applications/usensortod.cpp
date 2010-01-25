/**
 *	\file	examples/application/usensoravcc.cpp
 *	\brief	Example illustrating usage of sensor classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */
 
#ifdef __AVR_ATmega128__
#	define CPU_FREQUENCY 8000000UL
#else
#	define CPU_FREQUENCY 16000000UL
#endif
#define F_CPU CPU_FREQUENCY
// #define ALWAYS_INLINE_DELAY
// #define NO_INLINE_DELAY
#include "avr-halib/avr/interrupt.h"

#include "avr-halib/share/delay.h"
#include "avr-halib/share/cdevice.h"
#include "avr-halib/share/cbuffer.h"

#include "avr-halib/avr/uart.h"
UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);

#include "avr-halib/avr/twi.h"

struct RBoardController
{
	enum
	{
#ifdef __AVR_ATmega128__
		controllerClk=8000000
#else
		controllerClk=16000000
#endif
	};
};

struct UartConfiguration:public Uart1<RBoardController>
{
	enum{
// 	baudrate=1200
	baudrate=19200
// 	baudrate=115200
// 	baudrate=230400
	};
}; 
	
CDevice< SecOut< CInBuffer< COutBuffer< Uart< UartConfiguration >,uint8_t,255>,uint8_t,20 > > > cdev;

int main()
{
	delay_ms(64);
	TWIMaster< TWI<RBoardController> > twi;
	twi.init(128);
	sei();
	cdev << twi.st() << "\t";
	cdev << "Reset! Test: 4 3 2 1\t";
	cdev << twi.st() << "\t\n\r";
	while(true)
	{
// 		cdev << ((twi.start(104,false))?"wstart":"wserr") << twi.st() << "\t";
		cdev << (twi.start(104,false)) << "\t";
		cdev << twi.st() << "\t";
		cdev << ((twi.write(0))?"sadd":"Err:sadd") << "\t";
		cdev << twi.st() << "\t";
		cdev << ((twi.start(104,true))?"wstart":"rserr")<< "\t";
		cdev << twi.st() << "\t";
		cdev.writeNewline();
		
		for( int i = 6 ; i ; i--)
			cdev << (int)twi.read(false)<< "\t";
		
		cdev << (int)twi.read(true);
		twi.stop();
		cdev << "-Stop";
		{ //echo
			char c;
			while(cdev.get(c))
				cdev.put(c);
		}
		
 		cdev.writeNewline();
		
		delay_ms(200);
	}
// #endif	
}
