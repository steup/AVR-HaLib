/**
 *	\file	examples/application/usensoravcc.cpp
 *	\brief	Example illustrating usage of sensor classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */
 
#include "avr-halib/share/delay.h"
#include "avr-halib/share/cdevice.h"
#include "avr-halib/share/cbuffer.h"

#include "avr-halib/avr/uart.h"
#include "avr-halib/avr/twi.h"

struct UartConfig : public avr_halib::drivers::Uart::DefaultConfig
{
    static const bool useInterrupt = true;
};

typedef avr_halib::drivers::Uart::configure<UartConfig>::type Uart;

CDevice< SecOut< CInBuffer< COutBuffer< Uart, uint8_t, 255>, uint8_t, 20 > > > cdev;

int main()
{
	delay_ms(64);
	TWIMaster< TWI<RBoardController> > twi;
	twi.init(128);
	sei();
	cdev << (int32_t)twi.st() << "\t";
	cdev << "Reset! Test: 4 3 2 1\t";
	cdev << (int32_t)twi.st() << "\t\n\r";
	while(true)
	{
// 		cdev << ((twi.start(104,false))?"wstart":"wserr") << (int32_t)twi.st() << "\t";
		cdev << (int32_t)(twi.start(104,false)) << "\t";
		cdev << (int32_t)twi.st() << "\t";
		cdev << (int32_t)((twi.write(0))?"sadd":"Err:sadd") << "\t";
		cdev << (int32_t)twi.st() << "\t";
		cdev << (int32_t)((twi.start(104,true))?"wstart":"rserr")<< "\t";
		cdev << (int32_t)twi.st() << "\t";
		cdev.writeNewline();
		
		for( int i = 6 ; i ; i--)
			cdev << (int32_t)twi.read(false)<< "\t";
		
		cdev << (int32_t)twi.read(true);
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
