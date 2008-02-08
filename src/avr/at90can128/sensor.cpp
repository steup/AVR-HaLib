/*!
 *	\file	halib/at90can128/sensor.cpp
 *	\brief	Implementiert Analog- und Digital-Sensoren.
 *	\see \ref halibsensoren
 */

#include "halib/at90can128/interrupt.h"

#include <avr/interrupt.h>


/// Abstrakte Interrupt-Klasse f&uuml;r den AD-Wandler-Conversion-Complete-Interrupt
DECLARE_INTERRUPT_CLASS(InterruptAdConvComplete)
IMPLEMENT_INTERRUPT_CLASS(SIG_ADC, InterruptAdConvComplete)


/*! \class	AnalogDigitalConverter
 *	\brief	Analog-Digital-Wandler des ATmega128.
 *	
 *	Analog-Digital-Wandler des ATmega32. Interne Klasse, wird von AnalogSensor verwendet.
 *	\see \ref halibtimer
 */
class AnalogDigitalConverter : public InterruptAdConvComplete
{
private:
	/// Zeiger, der auf Wert-Puffer des gerade abgefragten Sensors zeigt. Null, wenn im Moment keine AD-Wandlung im Gange ist.
	static volatile uint8_t * target;
	/// Zeiger, der auf Variable des gerade abgefragten Sensors zeigt, die angibt, ob bereits ein neuer Sensorwert vorliegt. Null, wenn keine Wandlung im Gange.
	static volatile bool * done;

public:

	/*!	Startet eine AD-Wandlung.
	*	\param mux	Sensor-Kanal, an dem das Signal anliegt (0-3).
	*	\param ziel	Zeiger auf Speicherstelle, an die der abgefragte Wert geschrieben werden soll.
	*	\param fertig	Zeiger auf Speicherstelle, an die zur Best&auml;tigung des Vorliegens eines neuen Wertes true geschrieben werden soll.
	*	\return Gibt true zur&uuml;ck, wenn die AD-Wandlung gestartet werden konnte, also keine andere Wandlung mehr im Gange war.
	*/
	static bool startReadValue(uint8_t mux, volatile register_t * ziel, volatile bool * fertig);
	
	/*!	ISR des AD-Wandler-Conversion-Complete-Interrupt. Schreibt ermittelten Wert in Sensor-Puffer und gibt AD-Wandler f&uuml;r neue Konvertierung frei.
	*/
	void onInterruptAdConvComplete();
	
} adc;


volatile uint8_t * AnalogDigitalConverter::target = 0;
volatile bool * AnalogDigitalConverter::done = 0;



bool AnalogDigitalConverter::startReadValue(uint8_t mux, volatile register_t * ziel, volatile bool * fertig)
{
	if (target != 0)
		return false;			// AD-Wandlung im Gange
	target = ziel;
	done = fertig;
	*done = false;
	ADCSRA &= ~(1 << ADEN);			// Bit 7: Analog-Digital-Wandler disable: 1. Messung nach MUX-Wechsel bringt besseren Wert
	ADMUX = mux|(1<<ADLAR)| (0 << REFS1) | (1 << REFS0);			// AD-Kanal, Ausgabe linksbndig, Vergleichsspannung
	ADCSRA = (1 << ADEN);			// Bit 7: Analog-Digital-Wandler enable ADEN
	for (volatile uint8_t i = 5; i; i--)	// zum Einschwingen des Wandlers
		;
	ADCSRA |= (1 << ADSC)|(1 << ADIE);	// Starte AD-Wandlung ADSC, Interupt enable
	sei();
	return true;	
}

void AnalogDigitalConverter::onInterruptAdConvComplete()
{
	*target = ADCH;
	target = 0;
	*done = true;
	done = 0;
}


AnalogSensor::AnalogSensor(const uint8_t connectTo) :
	Sensor (false, 0),
	connect(connectTo)
{
	PORTF &= ~(1<<(connectTo));		// no pullup
	DDRF &= ~(1<<(connectTo));		//direction=read 
}

bool AnalogSensor::startGetValue()
{
	return AnalogDigitalConverter::startReadValue(connect, &value, &ready);
}


DigitalSensor::DigitalSensor(const uint8_t connectTo) :
	Sensor (true, 0),
	connect(connectTo)
{
	PORTF &= ~(1<<(connectTo));		// no pullup
	DDRF &= ~(1<<(connectTo));		//direction=read
	startGetValue();
}

bool DigitalSensor::startGetValue()
{
	value = (1 << (PB0+connect)) & PINB != 0 ? 0xff : 0;
	return true;
}

