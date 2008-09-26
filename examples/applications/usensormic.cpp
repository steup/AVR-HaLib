/**
 *	\file	examples/application/usensor.cpp
 *	\brief	Example illustrating usage of sensor classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#define CPU_FREQUENCY 16000000UL
#define F_CPU CPU_FREQUENCY

#include "avr-halib/avr/uart.h"
#include "avr-halib/ext/sensor.h"
#include "avr-halib/share/cdevice.h"
#include "avr-halib/share/delay.h"
#include "avr-halib/share/syncsensor.h"

#include "avr-halib/ext/ledblock.h"
#include "avr-halib/portmaps/robbyboard.h"



UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_ADC);

struct RBoard
{
	enum
	{
		controllerClk=16000000
	};

};


struct TestSensor
{
	typedef	ADConv<RBoard>	ADConverter;
	typedef	uint8_t	ReturnType;
	enum
	{
		mux = 0x0b,
		refV = (ADConverter::ref_internal2_56),
// 		refV = (ADConverter::ref_avcc),
		prescaler = (ADConverter::recommendedPrescalar/2 )
// 		prescaler = 0
		
	};
};

int main()
{
	
	SyncSensor< AnalogSensor< TestSensor > > as;
	
// 	CDevice< Uart< Uart1 > > uart(115200);
	Uart< Uart1 > uart(57600);
// 	LedBlock<LedBlock0123> leds;
	sei();
// 	uart << "Reset! Messungen: 4 3 2 1\n\r";
	
	while(true)
	{
// 		uint8_t a;
// 		uart.put('a');
// 		for (int i = 0; i<77; i++)
			{
				uart.put((char) as.getValue()); 
// 				leds.set(a>>3);
				delay_us(100);
			}
// 		uart.put('\n');
// 		uart.put('\r');
		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
}
