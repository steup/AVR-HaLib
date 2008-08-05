/**
 *	\brief	
 *	\author	
 *	\date	
 */
#define CPU_FREQUENCY 16000000UL
#define F_CPU CPU_FREQUENCY

#include "avr-halib/avr/uart.h"
#include "avr-halib/ext/sensor.h"
#include "avr-halib/share/cdevice.h"
// #include "avr-halib/share/delay.h"
#include "avr-halib/share/syncsensor.h"




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
	typedef	uint16_t	ReturnType;
	enum
	{
		mux = 5,
		refV = (ADConverter::ref_avcc),
		prescaler = (ADConverter::recommendedPrescalar)
		
	};
};

struct TestSensor2
{
	typedef	ADConv<RBoard>	ADConverter;
	typedef	uint16_t	ReturnType;
	enum
	{
		mux = 4,
		refV = (ADConverter::ref_internal2_56),
		prescaler = (ADConverter::recommendedPrescalar)
		
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
	
	SyncSensor< AnalogSensorInterrupt< TestSensor > > as;
	SyncSensor< AnalogSensor< TestSensor2 > > as2;
	
	CDevice< Uart< Uart1 > > uart;
	sei();
	uart << "Reset! Messungen: 4 3 2 1\n\r";
	
	while(true)
	{
		for (int i = 0; i<6; i++)
			uart << as.getValue() << "\t" << as2.getValue() << "\t";

 		uart.writeNewline();
		
		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
}
