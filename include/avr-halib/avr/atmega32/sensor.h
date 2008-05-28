/*!
 *	\file	avr-halib/atmega32/sensor.h
 *	\brief	Stellt eine Analog- und eine Digital-Sensor-Klasse f&uuml;r ATmega32/BobbyBoard bereit.
 *	\see \ref avr-halibsensoren
 */


/*! \class DigitalSensor
 *	\brief Implementiert Sensor-Interface f&uuml;r digitale Eing&auml;nge.
 *	\see \ref avr-halibsensoren
 */
class DigitalSensor : public Sensor
{
	protected:
		const uint8_t connect;
		
	public:
		/*! Konstruktor.
		*	\param connectTo	Als Eingang abzufragender Pin des PortB (0-7).
		*/
		DigitalSensor(const uint8_t connectTo);
		
		bool startGetValue();
};




/*! \class AnalogSensor
 *	\brief Implementiert Sensor-Interface f&uuml;r analoge Eing&auml;nge.
 *	\see \ref avr-halibsensoren
 */
class AnalogSensor : public Sensor
{
	protected:
		const uint8_t connect;

	public:
		/*! Konstruktor.
		*	\param connectTo	Nummer des analogen Eingangs (MUX, 0-3).
		*/
		AnalogSensor(const uint8_t connectTo);
		
		bool startGetValue();
};
