/**
 *	\file	examples/application/usensorsht.cpp
 *	\brief	Example illustrating usage of sensor classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#define CPU_FREQUENCY 16000000UL


#include "avr-halib/ext/sensor.h"
#include "avr-halib/share/syncsensor.h"
#include "avr-halib/ext/sht.h"

#include "avr-halib/share/delay.h"
#include "avr-halib/share/cdevice.h"

#include "avr-halib/avr/uart.h"
UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);

#include "avr-halib/ext/lcd_hd44780.h"
// #include "avr-halib/ext/lcm_16x4.h"
#include "avr-halib/portmaps/lcd_hd44780.h"

#include "avr-halib/avr/digitalout.h"

// #include "avr-halib/portmaps/sht.h"
#include "avr-halib/portmaps/robbyboard.h"




struct RBoard
{
	enum
	{
		controllerClk=16000000
	};

};

struct AVCCSensor
{
	//this Sensor config gives you (1.1 / avcc * 1024), avcc is vcc in many cases, higer value -> lower avcc
	typedef	ADConv<RBoard>	ADConverter;
	typedef	uint16_t	ReturnType;
	enum
	{
		mux = 0x1e,	//1.1V BandGab reference 
		refV = (ADConverter::ref_avcc),
		prescaler = (ADConverter::recommendedPrescalar)
		
	};
};


int main()
{
	DigitalOut<SensorPowerSupply> power;
	delay_ms(64);
	
	SyncSensor< AnalogSensor< AVCCSensor > > asvcc;


#if 0
	CDevice< Uart< Uart1<> > > cdev;
	sei();	
	cdev << "Reset! Messungen: 4 3 2 1\n\r";
	while(true)
	{
	//zur verwendung mit uart
// 	for (int i = 0; i<6; i++)
		cdev << as.getValue() << "\t"<< as2.getValue() << "\t"<< asvcc.getValue() << "\t";
		
		
		
 		cdev.writeNewline();
		
		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
#endif	

#if 1
 	CDevice< Uart< Uart1<> > > udev;
	sei();
	COutDevice< LcdHd44780< LcdHd44780Board > > cdev;
// 	COutDevice< Lcm_16x4< LcdHd44780Board > > cdev;
	while(true)
	{
	//zur verwendung mit LCD
// 	cdev.setPos(16);
// 	cdev << asvcc.getValue() << "  ";
	cdev << "zeile1 \n";
// 	udev << cdev.getPosi();
	cdev << "zeile2 \n";
// 	udev << cdev.getPosi();
	cdev << "zeile3 \n";
// 	udev << cdev.getPosi();
	cdev << "zeile4 \n";
// 	udev << cdev.getPosi() << "\n\r";
	
	for (volatile uint32_t i = 50000; i; i--) ;//warten
	
	cdev.clear();
	
	}
#endif

}
