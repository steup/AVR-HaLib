/*!	\file halib/timer.cpp
*	\brief	Implementiert Timer
*
*	... indem es die Implementierung für die passende Plattform included.
*
*	\see halib/atmega32/timer.cpp
*	\see halib/at90can128/timer.cpp
*/


#include <avr/interrupt.h>

#include "halib/config.h"
#include "halib/timer.h"



#if defined (__AVR_ATmega32__)
#	include "./atmega32/timer.cpp"
#else
#ifdef __AVR_AT90CAN128__
#	include "./at90can128/timer.cpp"
#else
#	error "device type not defined or no timers defined for this device"
#endif
#endif







/*
Timer::Timer() :
	firstEvent(0)
{
}


void Timer::addTimerEvent(TimerEvent & newEvent)
{
	// Neues Event vorn an die Liste hängen
	newEvent.nextEvent = firstEvent;
	firstEvent = & newEvent;
}


void Timer::removeTimerEvent(TimerEvent & event)
{
	// Event aus der Liste entfernen
	if (firstEvent == &event)
	{
		firstEvent = event.nextEvent;
	}
	else
	{
		TimerEvent * cur = firstEvent;
		while (cur != 0)
		{
			if (cur->nextEvent == &event)
			{
				// Gefunden! cur->nextEvent aus Liste entfernen
				cur->nextEvent = event.nextEvent;
				break;
			}
			cur = cur->nextEvent;
		}
	}
}


TimerEvent::TimerEvent(uint8_t dt, TimerEventHandler onTimerEvent) :
	nextEvent(0),
	eventTimerTicks(dt),
	onTimerEventHandler(onTimerEvent)
{
}


void TimerEvent::timerInterrupt(uint8_t i)
{
	if (eventTimerTicks && i % eventTimerTicks == 0 && onTimerEventHandler)
		(this->*onTimerEventHandler)();
	if (nextEvent)
		nextEvent->timerInterrupt(i);
}*/
