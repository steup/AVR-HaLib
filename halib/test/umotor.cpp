/**
 *	\brief	Motortestprogramm (Fernsteuerung per UART)
 *	\author	Philipp Werner
 *	\date	28.11.2007
 *
 *	\todo	Motor in halib einbauen, erweitern fuer mehrere Plattformen
 */



#include "halib/uart.h"

#include <avr/io.h>


#if !defined(__AVR_AT90CAN128__)
#	error "Nur fuer RobbyBoard ausgelegt!"
#endif


#include "../../taru/src/robbyat90can128/hardpwmmotordriver.cpp"


RobbyMotorA lmotor;
RobbyMotorB rmotor;

int8_t speed = 100;


class RemoteControl : public Uart<uint8_t, 100, 5>
{
public:
	void onInterruptUart0Recv();
};




void RemoteControl::onInterruptUart0Recv()
{
	// Standard-Uart-Interrupt-Behandlung durchfuehren
	Uart<uint8_t, 100, 5>::onInterruptUart0Recv();

	rmotor.setCalibrationFactor(-100);
	lmotor.setCalibrationFactor(100);


	
	switch (getc())
	{
		case '-':
			speed -= 10;
			if (speed < -100)
				speed = -100;
			(*this) << "Speed=" << speed << " (ab naechster Aenderung)\n\r";
			break;
		case '+':
			speed += 10;
			if (speed > 100)
				speed = 100;
			(*this) << "Speed=" << speed << " (ab naechster Aenderung)\n\r";
			break;
		case '8':				// Vorwaerts
		case 'w':
			lmotor.setSpeed(speed);
			rmotor.setSpeed(speed);
			sout("VOR\n\r");
			break;
		case '4':				// Links
		case 'a':
			lmotor.setSpeed(-speed);
			rmotor.setSpeed(speed);
			sout("LINKS\n\r");
			break;
		case '5':				// Stop
		case ' ':
			lmotor.setSpeed(0);
			rmotor.setSpeed(0);
			sout("STOPP\n\r");
			break;
		case '6':				// Rechts
		case 'd':
			lmotor.setSpeed(speed);
			rmotor.setSpeed(-speed);
			sout("RECHTS\n\r");
			break;
		case '2':				// Rueckwaerts
		case 's':
			lmotor.setSpeed(-speed);
			rmotor.setSpeed(-speed);
			sout("ZURUECK\n\r");
			break;

		case 't':
			lmotor.setSpeed(speed);
			sout("LINKS VOR\n\r");
			break;
		case 'g':
			lmotor.setSpeed(0);
			sout("LINKS STOPP\n\r");
			break;
		case 'b':
			lmotor.setSpeed(-speed);
			sout("LINKS RUECK\n\r");
			break;
		case 'z':
			rmotor.setSpeed(speed);
			sout("RECHTS VOR\n\r");
			break;
		case 'h':
			rmotor.setSpeed(0);
			sout("RECHTS STOPP\n\r");
			break;
		case 'n':
			rmotor.setSpeed(-speed);
			sout("RECHTS RUECK\n\r");
			break;
		case '/':
			(*this) << "Status:\n\rrechts -> cali = " << rmotor.getCalibrationFactor() << "; speed = " << rmotor.getSpeed() <<
			                  "\n\rlinks  -> cali = " << lmotor.getCalibrationFactor() << "; speed = " << lmotor.getSpeed() << "\n\r";
			break;

		case 0:
			break;
	
		default:
			sout("?\n\r");
		break;
	}
	
}

int main()
{
 	RemoteControl rc;
 	
	rc << "Taru betriebsbereit!\n\r";

	while (1)
		;
	
	return 0;
}
