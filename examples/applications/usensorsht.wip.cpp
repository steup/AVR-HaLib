/**
 *	\brief	Sensor-Test mit Uart
 *	\author	Philipp Werner
 *	\date	27.11.2007
 */
#define CPU_FREQUENCY 16000000UL
#define F_CPU CPU_FREQUENCY

// #include "halib/avr/uart.h"
// #include "halib/avr/adc.wip.h"
// #include "halib/avr/sensor.wip.h"
#include "halib/share/cdevice.h"
// #include "halib/share/delay.h"
#include "halib/share/simplifysensor.h"
#include "halib/ext/sht.h"
#include "halib/ext/SHT.portmap.h"
#include "halib/ext/lcd_hd44780.h"
#include "halib/ext/lcd_hd44780.portmap.h"

// UseInterrupt(SIG_UART1_RECV);
// UseInterrupt(SIG_UART1_DATA);


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
	
	SimplifySensor< SHTTemperatur< SHTfront > > as;
	SimplifySensor< SHTHumidity< SHTfront > > as2;	

// 	CDevice< Uart< Uart1 > > cdev;
// 	sei();	
	
	COutDevice< LcdHd44780< LcdHd44780Board > > cdev;
	
// 	cdev << "Reset! Messungen: 4 3 2 1\n\r";
	
	while(false)
	{
	//zur verwendung mit uart
	for (int i = 0; i<6; i++)
		cdev << as.getValue() << "\t"<< as2.getValue() << "\t";
		
		
		
 		cdev.writeNewline();
		
		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
	
	while(true)
	{
	//zur verwendung mit LCD
		cdev.setPos(0);
		cdev << as.getValue() - 4000 << "  "; // Temperatur in grad C * 100 // vcc 5V
		
		cdev.setPos(64);
		cdev << as2.getValue() * 4 - 400 << "  "; // Relative Luftfeutigkeit in % * 100 //Umrechnung näherungsweise nur mit liniaren anteil 

		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
}
