/**
 *	\brief	Sensor-Test mit Uart
 *	\author	Philipp Werner
 *	\date	27.11.2007
 */
#define CPU_FREQUENCY 16000000UL
#define F_CPU CPU_FREQUENCY

#include "halib/avr/uart.h"
// #include "halib/avr/adc.wip.h"
// #include "halib/avr/sensor.wip.h"
#include "halib/share/cdevice.h"
// #include "halib/share/delay.h"
#include "halib/share/simplifysensor.h"
#include "halib/ext/sht.h"
#include "halib/ext/SHT.portmap.h"

UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);


struct RBoard
{
	enum
	{
		controller_clk=16000000
	};

};

int main()
{
	DDRA=0xff;
	DDRF=0x00;
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
	
	SimplifySensor< SHTTemperatur< SHTlcd > > as;
	SimplifySensor< SHTHumidity< SHTlcd > > as2;	
		CDevice< Uart< Uart1 > > uart;
	sei();
	uart << "Reset! Messungen: 4 3 2 1\n\r";
	
	while(true)
	{
	for (int i = 0; i<6; i++)
		uart << as.getValue() << "\t"<< as2.getValue() << "\t";

 		uart.writeNewline();
		
		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
}
