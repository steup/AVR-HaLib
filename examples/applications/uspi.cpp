/**
 *	\file	examples/application/usensorsht.cpp
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


#include "avr-halib/ext/sensor.h"
#include "avr-halib/share/syncsensor.h"
#include "avr-halib/ext/sht.h"

#include "avr-halib/share/delay.h"
#include "avr-halib/share/cdevice.h"

#include "avr-halib/avr/uart.h"
UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_UART0_RECV);
UseInterrupt(SIG_UART0_DATA);
// #include "avr-halib/ext/lcd_hd44780.h"
// #include "avr-halib/portmaps/lcd_hd44780.h"

#include "avr-halib/avr/spi.h"


#include "avr-halib/avr/digitalout.h"

// #include "avr-halib/portmaps/sht.h"
// #include "avr-halib/portmaps/robbyboard.h"

#ifdef __AVR_ATmega128__
#include "avr-halib/portmaps/chipconboard.h"
#endif

			 
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
	typedef RBoardController Controller_Configuration;
	enum{baudrate=19200};
}; 

struct SpiConfiguration:public Spi<RBoardController>
{
	typedef RBoardController Controller_Configuration;
	enum{
		useInterupt=false,
		dataDirection=msb,
		leadingEdge=rising,
		setupEdge=leading,
		clockPrescaler=ps64
		/*,busywaitput=true*/
	};
};
int main()
{
// 	DigitalOut<SensorPowerSupply> power;
// 	power.setOff();
	delay_ms(64);

// 	CDevice< Uart< Uart1< RBoardController > > > cdev;
// 	CDevice< Uartnoint<  UartConfiguration > > cdev;
	CDevice< SecOut<Uart< UartConfiguration > > > cdev;
// 	CDevice< Uart< Uart1 > > cdev;

#ifdef __AVR_ATmega128__	
	UseRegmap(cc2420,CC2420);
	cc2420.reset.ddr =true;
	cc2420.reset.port =true;
#endif	
	
	SpiMaster< SpiConfiguration > spi;
	
	sei();	
	cdev << "Reset! Messungen: 4 3 2 1\n\r";
	while(true)
	{
		char c;
		if (cdev.get(c))
		{
			spi.put(c);
 			spi.get(c);
			cdev.put(c);
		}
				
		for (volatile uint32_t i = 500; i; i--) ;//warten
	}


}
