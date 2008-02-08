/**
 *	\file	halib/at90can128/motor.cpp
 *	\brief	Implementiert die Motor-Klasse und Motor-PWM.
 *
 *	\see \ref halibmotoren
 *
 */


#include "halib/at90can128/motor.h"
#include "halib/timer.h"

#define ABS(x)	((x) < 0 ? (-(x)) : (x))



const uint8_t pwmMin = 0;
const uint8_t pwmMax = 255;

/* \brief	Betreibt PWM f�r Motoren.
 *	
 *	Notwendig zur Implementierung von Motor.
 *
 *	\see \ref halibmotoren
 */
class MotorPwm : public Timer2
{
public:

	static uint8_t counter;
	static uint8_t nextEvent;

	static uint8_t stepCount;
	static uint8_t stepsWidth;

	static bool isRunning;


	void determineNextStep();
	
	void onTimer();
	void event();

} motorPwm;


uint8_t MotorPwm::counter = pwmMin;
uint8_t MotorPwm::nextEvent = pwmMin;
uint8_t MotorPwm::stepCount = 0;
uint8_t MotorPwm::stepsWidth = 0;
bool    MotorPwm::isRunning = false;


static Motor * firstMotor = 0;


Motor::Motor(volatile port_t & p, const port_t em, const port_t fm, const port_t bm) :
	nextMotor(firstMotor),
	calibrationFactor(127),
	speed(0),
	absSpeed(0),
	port(p),
	enableMask(em),
	forwardMask(fm),
	backwardMask(bm)
{
	firstMotor = this;
}


void MotorPwm::determineNextStep()
{
	// wieviel Counter-Einheiten noch bis zum n�chsten Schalten eines Motors / Reset
	const uint8_t dif = counter - nextEvent;
	
	// H�chsten verwendbaren Prescaler ausw�hlen
	uint8_t prescaler;
	uint8_t prescalerId;
/*	if (dif >= 128)
	{
		prescaler = 128;
		prescalerId = TIMER_PS_128;
	}
	else  if (dif >= 64)
	{
		prescaler = 64;
		prescalerId = TIMER_PS_64;
	}
	else if (dif >= 32)
	{
		prescaler = 32;
		prescalerId = TIMER_PS_32;
	}
	else */ if (dif >= 8)
	{
		prescaler = 8;
		prescalerId = TIMER_PS_8;
	}
	else
	{
		prescaler = 1;
		prescalerId = TIMER_PS_1;
	}	
	
	// Alle wieviel Timer-Interrupts soll der Handler aufgerufen werden
	stepCount = dif / prescaler;
	// So viele Counter-Einheiten werden bis zum n�chsten Aufruf vergangen sein
	stepsWidth = prescaler * stepCount;
	
	start(prescalerId);
}


void MotorPwm::event()
{
	Motor * m = firstMotor;
	uint8_t max = 0;

	if (counter == pwmMin)
	{
		// Alle Motoren abschalten und Maximalgeschwindigkeit fr naechsten Durchlauf suchen
		while (m)
		{
			if (m->absSpeed != pwmMax)
			{
				m->disable();
				if (m->absSpeed > max)
					max = m->absSpeed;
			}
			m = m->nextMotor;
		}
	}
	else
	{
		// Entsprechende Motoren anschalten und Maximalgeschwindigkeit fuer naechsten Durchlauf suchen
		while (m)
		{
			if (m->absSpeed == nextEvent)
				m->enable();
			else if (m->absSpeed < nextEvent && m->absSpeed > max)
				max = m->absSpeed;
			m = m->nextMotor;
		}
	}

	nextEvent = max;
	determineNextStep();
}



void MotorPwm::onTimer()
{
	stepCount--;
	if (!stepCount)
	{
		counter -= stepsWidth;
		if (counter == nextEvent)
			event();
		else
			determineNextStep();
	}
}




void Motor::stopAll()
{
	Motor * m = firstMotor;
	while (m)
	{
		m->stop();
		m->speed = 0;
		m->absSpeed = 0;
		m = m->nextMotor;
	}
	
	motorPwm.stop();
	motorPwm.isRunning = false;
}

void Motor::setSpeed(int8_t s)
{
	if (s == -128)
		s = -127;

	if (speed == s)
		return;

	speed = s;

	speedChanged();
}

void Motor::speedChanged()
{
	if (speed == 0)
	{
		absSpeed = 0;
		stop();
		
		// Laeuft noch einer der Motoren?
		bool noMotorRunning = true;
		Motor * m = firstMotor;
		while (m)
		{
			if (m->absSpeed != 0)
			{
				noMotorRunning = false;
				break;
			}
			m = m->nextMotor;
		}
		
		// Wenn nein, dann Timer/Pwm deaktiveren
		if (noMotorRunning)
		{
			motorPwm.stop();
			motorPwm.isRunning = false;
		}
	}
	else
	{
		const int8_t cali = ((int16_t)speed) * ((int16_t)calibrationFactor) / ((int16_t)127);
		absSpeed = ABS(cali) * 3 / 2 + 65;
		
		if (speed < 0)
			backward();
		else
			forward();

		enable();

		if (absSpeed != pwmMax)
		{
		
			// Wenn noch keiner der Motoren lief PWM anstossen
			if (!motorPwm.isRunning)
			{
				motorPwm.counter = 0;
				motorPwm.nextEvent = 0;
			
				motorPwm.event();
				motorPwm.isRunning = true;
			}
		}
	}
}

