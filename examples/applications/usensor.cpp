/**
 *	\brief	Sensor-Test mit Uart
 *	\author	Philipp Werner
 *	\date	27.11.2007
 */

#include "halib/uart.h"
#include "halib/sensor.h"
#include "halib/misc.h"



int main()
{
	AnalogSensor s0(0);
	AnalogSensor s1(1);
	AnalogSensor s2(2);
	AnalogSensor s3(3);
	
#if defined(__AVR_AT90CAN128__)
	// Robby-Board
	DigitalOut power(PORTC, DDRC, 0xff); // Strom fuer die Sensoren, 0 ist an
#elif defined(__AVR_ATMEGA32__)
	// Bobby-Board
	DigitalOut power(PORTD, DDRD, 0x0c);
#else
#	error "Board not supported"
#endif
	power.replace(0x00);

	Uart<uint16_t, 1000,10> uart;
	
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
