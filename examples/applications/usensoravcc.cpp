/**
 *	\file	examples/application/usensoravcc.cpp
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
// #define ALWAYS_INLINE_DELAY
// #define NO_INLINE_DELAY

#include "avr-halib/avr/interrupt.h"
#include "avr-halib/ext/sensor.h"
#include "avr-halib/share/syncsensor.h"

#include "avr-halib/share/delay.h"
#include "avr-halib/share/cdevice.h"

#include "avr-halib/avr/uart.h"
UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_UART0_RECV);
UseInterrupt(SIG_UART0_DATA);
// #include "avr-halib/ext/lcd_hd44780.h"
// #include "avr-halib/portmaps/lcd_hd44780.h"

// #include "avr-halib/avr/digitalout.h"

// #include "avr-halib/portmaps/robbyboard.h"

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

struct AVCCSensor
{
	//this Sensor config gives you (1.1 / avcc * 1024), avcc is vcc in many cases, higer value -> lower avcc
	typedef	ADConv<RBoardController>	ADConverter;
	typedef	uint16_t	ReturnType;
	enum
	{
		mux = 0x1e,	//1.1V BandGab reference 
		refV = (ADConverter::ref_avcc),
		//ref_internal2_56, ref_aref
		prescaler = (ADConverter::recommendedPrescalar)
		
	};
};

struct UartConfiguration:public Uart1<RBoardController>
{
	enum{
// 	baudrate=19200
// 	baudrate=115200
	baudrate=230400
	};
}; 
	
	CDevice< Uart< UartConfiguration > > cdev;

int main()
{
// 	DigitalOut<SensorPowerSupply> power;
// 	power.setOff();
	
	delay_ms(64);
	
	SyncSensor< AnalogSensor< AVCCSensor > > asvcc;
	
// 	CDevice< Uartnoint< UartConfiguration > > cdev;
// 	CDevice< Uart< Uart1<> > > cdev;
// 	CDevice< Uart< Uart1<RBoardController,19200> > > cdev;

	sei();
	cdev << "Reset! Messungen: 4 3 2 1\n\r";
	while(true)
	{
	//zur verwendung mit uart
// 	for (int i = 0; i<6; i++)
		cdev <</* as.getValue() << "\t"<< as2.getValue() << "\t"<<*/(int32_t)((11UL*1023UL*100UL)/asvcc.getValue()) << "\t";
		
		{ //echo
			char c;
			if (cdev.get(c))
				cdev.put(c);
		}
		
 		cdev.writeNewline();
		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
		delay_ms(200);
	}
// #endif	

#if 0
 	COutDevice< LcdHd44780< LcdHd44780Board > > cdev;
	while(true)
	{
	//zur verwendung mit LCD
		cdev.setPos(0);
		cdev << as.getValue() - 4000 << "  "; // Temperatur in grad C * 100 // vcc 5V
		
		cdev.setPos(64);
		cdev << as2.getValue() * 4 - 400 << "  "; // Relative Luftfeutigkeit in % * 100 //Umrechnung näherungsweise nur mit liniaren anteil 
		
		cdev.setPos(16);
		cdev << asvcc.getValue() << "  ";
		

		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
#endif

}
