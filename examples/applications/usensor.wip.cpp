/**
 *	\brief	Sensor-Test mit Uart
 *	\author	Philipp Werner
 *	\date	27.11.2007
 */
#define CPU_FREQUENCY 16000000UL

#include "halib/avr/uart.h"
#include "halib/avr/adc.wip.h"
#include "halib/share/cdevice.h"

UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);


struct RBoard
{
	enum
	{
		controller_clk=16000000
	};

};

AnalogDigitalConverter<uint16_t,ADConv,RBoard> ad;

uint16_t getValue(uint8_t mux)
{
	uint16_t a;
	PORTA = ad.getValue(a, mux, ADConv::ref_avcc)?0xff:0x00; //avcc,internal2_56
	return a;
}
	

int main()
{
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

	CDevice< Uart<Uart1> > uart;
	sei();
	uart << "Reset! Messungen: 4 3 2 1\n\r";
	
	while(true)
	{
	for (int i = 0; i<6; i++)
		uart << getValue(i) << "\t";

#if 0			
		uart << getValue(0) << "\t";
			
		uart << getValue(0) << "\t" << getValue(0) << "\t"<< getValue(0) << "\t"<< getValue(0)
			<< "\t|\t" << getValue(1) << "\t"<< getValue(1) << "\t"<< getValue(1) << "\t"<< getValue(1)
				<< "\t|\t" << getValue(5) << "\t"<< getValue(5) << "\t"<< getValue(5) << "\t"<< getValue(5)
				<< "\t|\t" << getValue(3) << "\t"<< getValue(3) << "\t"<< getValue(3) << "\t"<< getValue(3);
#endif		
		uart.writeNewline();
		
		for (volatile uint32_t i = 50000; i; i--)
			;
	}
}
