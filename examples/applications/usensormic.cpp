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
#include "avr-halib/avr/timer.h"


UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_ADC);

UseInterrupt(SIG_OUTPUT_COMPARE3A);

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
		prescaler = (ADConverter::recommendedPrescalar/8 )
// 		prescaler = 0
		
	};
};

// SyncSensor< AnalogSensor< TestSensor > > as;
AnalogSensorInterrupt< TestSensor > as;
Uart< Uart1 > uart(57600);
LedBlock<LedBlock0123> leds;
void get()
{
	leds.set(1);
// 	static uint8_t i = 0 ;
// 	if (i==3) 
	uint8_t value;
	bool gotv = as.getCachedValue(value);
	bool start = as.startGetValue();
	leds.set(2);
	uart.put((char) value);
// 	i++;
// 	i%=10;
	if (start) leds.set(4); else leds.set(8);
	
}


int main()
{
// 	CDevice< Uart< Uart1 > > uart(115200);
// 	LedBlock<LedBlock0123> leds;
	
// 	uart << "Reset! Messungen: 4 3 2 1\n\r";
	uart.put('h');
	uart.put('a');
	uart.put('l');
	uart.put('l');
	uart.put('o');
	uart.put('!');
	UseRegmap(timerregister, Timer3);
	
	Timer<Timer3> timer;
	
	as.init();
	
	timer.selectClock(Timer3::ps1);
	
	timer.setWaveformGenerationMode(Timer3::ctc);
	
	timerregister.outputCompareAH =(1000 >> 8);
	timerregister.outputCompareA = 1000;
	
	timer.setInterruptMask(Timer3::im_outputCompareAEnable);
	
	redirectISRF(SIG_OUTPUT_COMPARE3A, &get);
	sei();

	
	while(true); 
	{
// 		uint8_t a;
// 		uart.put('a');
// 		for (int i = 0; i<77; i++)
			{
// 				uart.put((char) as.getValue()); 
// 				leds.set(a>>3);
				delay_us(100);
			}
// 		uart.put('\n');
// 		uart.put('\r');
		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
	
}
