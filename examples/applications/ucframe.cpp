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
#include "avr-halib/share/cbuffer.h"
#include "avr-halib/share/cframe.h"

#include "avr-halib/avr/uart.wip.h"
// #include "avr-halib/avr/uart.h"
UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_UART0_RECV);
UseInterrupt(SIG_UART0_DATA);

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

char framedaten[200] ;
uint8_t len = 0 ;

struct UartConfiguration:public Uart1<RBoardController>
{
	enum{
// 	baudrate=1200
	baudrate=19200
// 	baudrate=115200
// 	baudrate=230400
	};
}; 

struct UartConfiguration2:public Uart0<RBoardController>
{
	enum{
// 	baudrate=1200
	baudrate=19200
// 	baudrate=115200
// 	baudrate=230400
	};
}; 

CDevice< SecOut<CFrame< Uart< UartConfiguration2 >, uint8_t > > > cframe;
CDevice< SecOut< CInBuffer< COutBuffer< Uart < UartConfiguration > ,uint8_t,255>,uint8_t,20 > > > cdev;

int main()
{
	delay_ms(64);
	
	sei();
	cdev << "start len stream\n\r";
	cframe << "start charstuffing\n\r";
	char in[200];
	uint8_t inpos=0;
	uint8_t inlen=0;
	char c;
	
	while(true)
	{
		SyncMem;
		if(inpos > inlen && inlen>0){ cframe.send(in, inlen); inpos = 0;}
		if (cdev.get(c))
		{
			cdev.put(c);
			if(inpos == 0) inlen = c;
			else in[inpos-1] = c;
			inpos++;
		}
		len = 200;
		if(cframe.recive(framedaten, len))
		{
			cdev << "\n\rrecive " << len <<"\t";
			for(uint8_t i = 0; i < len; i++ )
				cdev.put(framedaten[i]);
		}
		delay_ms(10);
	}
#if 0	
	{
		
		cdev <</* as.getValue() << "\t"<< as2.getValue() << "\t"<<*/(11UL*1023UL*100UL)/asvcc.getValue() << "\t";
		
		{ //echo
			char c;
			while(cdev.get(c))
				cdev.put(c);
		}
		
 		cdev.writeNewline();
		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
		delay_ms(200);
	}
#endif	
}
