/**
 *	\file	examples/application/umotor.cpp
 *	\brief	Example illustrating usage of Motor and Uart ("robot remote control")
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */


#if defined(__AVR_AT90CAN128__)
#	define CPU_FREQUENCY 16000000UL
#	include "avr-halib/portmaps/robbyboard.h"
#else
#	error "Example program not ported to this platform yet."
#endif


#include "avr-halib/share/cdevice.h"
#include "avr-halib/avr/uart.h"
#include "avr-halib/ext/motor.h"
#include "avr-halib/share/cbuffer.h"

UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);


// COutDevice< CInBuffer< COutBuffer< Uart< Uart1<> > uint8_t, 100> uint8_t, 5> > uart;
COutDevice< SecOut< CInBuffer< COutBuffer< Uart < Uart1<> > ,uint8_t,255>,uint8_t,20 > > > uart;

// RobbyMotorDriver uses Timer1 !!!!
Motor< RobbyMotorA<RobbyMotorDriver> > lmotor;
Motor< RobbyMotorB<RobbyMotorDriver> > rmotor;


int8_t speed = 100;



void onInterruptUartRecv()
{
	char c;

	// Standard-Uart-Interrupt-Behandlung durchfuehren
// 	uart.onUartRecv();

	rmotor.setCalibrationFactor(-100);
	lmotor.setCalibrationFactor(100);

	if (!uart.get(c))
		return;

	switch (c)
	{
		case '-':
			speed -= 10;
			if (speed < -100)
				speed = -100;
			uart << "Speed=" << (int32_t)speed << " (ab naechster Aenderung)\n\r";
			break;
		case '+':
			speed += 10;
			if (speed > 100)
				speed = 100;
			uart << "Speed=" << (int32_t)speed << " (ab naechster Aenderung)\n\r";
			break;
		case '8':				// Vorwaerts
		case 'w':
			lmotor.setSpeed(speed);
			rmotor.setSpeed(speed);
			uart.writeString("VOR\n\r");
			break;
		case '4':				// Links
		case 'a':
			lmotor.setSpeed(-speed);
			rmotor.setSpeed(speed);
			uart.writeString("LINKS\n\r");
			break;
		case '5':				// Stop
		case ' ':
			lmotor.setSpeed(0);
			rmotor.setSpeed(0);
			uart.writeString("STOPP\n\r");
			break;
		case '6':				// Rechts
		case 'd':
			lmotor.setSpeed(speed);
			rmotor.setSpeed(-speed);
			uart.writeString("RECHTS\n\r");
			break;
		case '2':				// Rueckwaerts
		case 's':
			lmotor.setSpeed(-speed);
			rmotor.setSpeed(-speed);
			uart.writeString("ZURUECK\n\r");
			break;

		case 't':
			lmotor.setSpeed(speed);
			uart.writeString("LINKS VOR\n\r");
			break;
		case 'g':
			lmotor.setSpeed(0);
			uart.writeString("LINKS STOPP\n\r");
			break;
		case 'b':
			lmotor.setSpeed(-speed);
			uart.writeString("LINKS RUECK\n\r");
			break;
		case 'z':
			rmotor.setSpeed(speed);
			uart.writeString("RECHTS VOR\n\r");
			break;
		case 'h':
			rmotor.setSpeed(0);
			uart.writeString("RECHTS STOPP\n\r");
			break;
		case 'n':
			rmotor.setSpeed(-speed);
			uart.writeString("RECHTS RUECK\n\r");
			break;
		case '/':
			uart << "Status:\n\rrechts -> cali = " << (int32_t)rmotor.getCalibrationFactor() << "; speed = " << (int32_t)rmotor.getSpeed() <<
			        "\n\rlinks  -> cali = " << (int32_t)lmotor.getCalibrationFactor() << "; speed = " << (int32_t)lmotor.getSpeed() << "\n\r";
			break;
		default:
			uart.writeString("?\n\r");
			break;
	}
}

int main()
{
	sei();
	uart << "betriebsbereit!\n\r";

// 	redirectISRF(SIG_UART1_RECV, & onInterruptUartRecv);
	uart.onReceive.bind< & onInterruptUartRecv >();
	uart.enableonReceive();
	while (1);
	
	return 0;
}
