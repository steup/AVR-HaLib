/*!	\file halib/sensor.h
*	\brief	Definiert AnalogSensor und DigitalSensor
*
*	... indem es die Definition für die passende Plattform included.
*
*	\see halib/atmega32/sensor.h
*/


#pragma once

#include "halib/config.h"

/*!	\brief Gemeinsames Interface fr AnalogSensor und DigitalSensor
*
*/
class Sensor
{
protected:
	volatile bool ready;
	volatile register_t value;
	
	Sensor(volatile bool r, volatile register_t v) :
		ready(r), value(v)
	{}
	
public:

	/// True, wenn mit startGetValue() gestarteter Wertholvorgang abgeschlossen.
	bool isReady()
	{
		return ready;
	}

	/*! \brief Beginnt das Auslesen eines neuen Sensorwertes.
	*	\return false, wenn grad kein neuer Wert ausgelesen werden kann.
	*
	*	Die Funktion kann fehlschlagen, wenn der AD-Wandler bereits beschäftigt
	*	ist.
	*/
	virtual bool startGetValue() = 0;

	/// Gibt zuletzt vom Sensor gelesenen Wert zurück.
	register_t getCachedValue()
	{
		return value;
	}

	/// Hole neuen Wert vom Sensor und gebe ihn zurück. Funktion wartet ggf. auf Fertigstellung der AD-Wandlung.
	register_t getValue()
	{
		while(!startGetValue());
		while(!ready);
		return getCachedValue();
	}
};


#ifdef __AVR_ATmega32__
#	include "halib/avr/atmega32/sensor.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "halib/avr/at90can128/sensor.h"
#else
#	error "device type not defined or no sensor implemented for this device"
#endif
#endif




