/**
 *	\brief	Sensor-Test mit Uart
 *	\author	Philipp Werner
 *	\date	27.11.2007
 */
#define CPU_FREQUENCY 16000000UL
#define F_CPU CPU_FREQUENCY

#include "halib/avr/uart.h"
#include "halib/avr/adc.h"
#include "halib/ext/sensor.h"
#include "halib/share/cdevice.h"
#include "halib/share/delay.h"
#include "halib/share/simplifysensor.h"


UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_ADC);

struct RBoard
{
	enum
	{
		controller_clk=16000000
	};

};

//AnalogDigitalConverterInterrupt<uint16_t, ADConv<RBoard> > ad;
AnalogDigitalConverter<uint16_t,ADConv<RBoard> > ad;



uint16_t getValue(uint8_t mux)
{
	uint16_t a = 0;
	PORTA = ad.getValue(a, mux, ADConv<RBoard>::ref_avcc,ADConv<RBoard>::recommendedPrescalar)?0xff:0x00; //avcc,internal2_56
	
		
// 	while (ad.isThatTarget(a))
// 	;
	PORTA = 0;
	return a;
}

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
	
	
	CDevice< Uart< Uart1 > > uart;
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
