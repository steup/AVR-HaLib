/**
 *	\brief	Sensor-Test mit Uart
 *	\author	Philipp Werner
 *	\date	27.11.2007
 */
#define CPU_FREQUENCY 16000000UL
#include "halib/avr/uart.h"
#include "halib/ext/sensor.h"



int main()
{
	AnalogSensor s0(0);
	AnalogSensor s1(1);
	AnalogSensor s2(2);
	AnalogSensor s3(3);
	
	
#if defined(__AVR_AT90CAN128__)
	// Robby-Board
	DDRC=0xff;
	PORTC=0x00;
#elif defined(__AVR_ATMEGA32__)
	// Bobby-Board
	DDRC=0x0c;
	PORTC=0x00;
#else
#	error "Board not supported"
#endif

	Uart<Uart1> uart;
	
	uart << "Reset! Messungen: 4 3 2 1\n\r";
	
	while(true)
	{
			
		uart << s0.getValue() << "\t"<< s0.getValue() << "\t"<< s0.getValue() << "\t"<< s0.getValue()
				<< "\t|\t" << s1.getValue() << "\t"<< s1.getValue() << "\t"<< s1.getValue() << "\t"<< s1.getValue()
				<< "\t|\t" << s2.getValue() << "\t"<< s2.getValue() << "\t"<< s2.getValue() << "\t"<< s2.getValue()
				<< "\t|\t" << s3.getValue() << "\t"<< s3.getValue() << "\t"<< s3.getValue() << "\t"<< s3.getValue();
		uart.newline();
		
		for (volatile uint32_t i = 50000; i; i--)
			;
	}
}
