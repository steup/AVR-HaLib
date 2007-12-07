/**
 *	\file halib/misc.h
 *	\brief	Definiert Digital-IO, Benutzerschnittstelle (LEDs und Buttons)
 *
 */

#pragma once
#include <avr/io.h>
#include "halib/config.h"




/**
 *	\brief Digitaler Ausgang
 */
class DigitalOut
{
protected:
	volatile port_t & oPort;
	const port_t oMask;

public:
	/**	Konstruktor.
	 *	@param port Referenz auf den Ausgangs-Ports.
	 *	@param ddr	Referenz auf Data Direction Register des Ports
	 *	@param mask Bitmaske, die Pins kennzeichnet ber die Ausgabe erfolgen soll.
	 */
	DigitalOut(volatile port_t & port, volatile port_t & ddr, const port_t mask);

	/**	Maskenbits auf Port setzen.
	 */
	inline void set()
	{
		oPort |= oMask;
	}

	/**
	 * Ersetzt die maskierten Bits des Ports durch die des Patterns
	 */
	void replace(const port_t pattern);
	
	/**	Maskenbits auf Port zurcksetzen.
	 */
	inline void unset()
	{
		oPort &= ~oMask;
	}
	
	/**	Maskenbits auf Port umschalten.
	 */
	inline void toggle()
	{
		oPort ^= oMask;
	}
	
	/**	Liefert den nach Bitmaske gefilterten Port-Status zurueck.
	 */
	inline port_t getStatus()
	{
		return oPort & oMask;
	}
};


#ifdef USE_LED

/**
 *	\brief	Einzelne LED
 */
class Led : protected DigitalOut
{
public:

	/**	Konstruktor.
	 *	@param port Referenz auf den Port, ber den die LED ansprechbar ist.
	 *	@param ddr	Referenz auf Data Direction Register des Ports
	 *	@param mask Bitmaske, die Pins kennzeichnet ber die die LEDs auf dem Port angesprochen werden k�nnen.
	 */
	Led(volatile port_t & port, volatile port_t & ddr, const port_t mask);

	/**	LED anschalten.
	 */
	inline void ledOn()
	{
#if defined (__AVR_ATmega32__)
		unset();
#elif defined (__AVR_AT90CAN128__)
		set();
#else
#	warning "defaulting to pos led"
		set();
#endif
	}
	
	/**	LED ausschalten.
	 */
	inline void ledOff()
	{
#if defined (__AVR_ATmega32__)
		set();
#elif defined (__AVR_AT90CAN128__)
		unset();
#else
#	warning "defaulting to pos led"
		unset();
#endif
	}
	
	/**	LED umschalten.
	 */
	inline void ledToggle()
	{
		toggle();
	}

	/**	Liefert true, wenn die LED angeschalten ist.
	 */
	inline bool isLedOn()
	{
#if defined (__AVR_ATmega32__)
		return !getStatus();
#elif defined (__AVR_AT90CAN128__)
		return getStatus();
#else
#	warning "defaulting to pos led"
		return getStatus();
#endif
	}
};

#endif

#ifdef USE_LEDBLOCK

/**
*	\brief	Block von LEDs (mehrere LEDs an einem Port)
*/
class LedBlock : protected DigitalOut
{
protected:
	uint8_t patternShift;
public:
	
	/**	Konstruktor.
	 *	@param port Referenz auf den Port, ber den die LEDs ansprechbar sind.
	 *	@param ddr	Referenz auf Data Direction Register des Ports
	*	@param mask Bitmaske, die Pins kennzeichnet ber den die LEDs auf dem Port angesprochen werden k�nen.
	 *	@param shift	Anzahl Bits, um die das pattern nach links geshiftet werden muss, um auf mask zu passen
	 */
	LedBlock(volatile port_t & port, volatile port_t & ddr, const port_t mask, const uint8_t shift);

	/**	Setzt die LEDs nach einem Muster.
	 *	@param pattern	Bitmuster, das auszugebendes LED-Muster repr�sentiert (1 f�r LED an)
	 */
	void setLedPattern(const uint8_t pattern);
	
	/**	Gibt das aktuelle LED-Muster zurck.
	 *	@return	Bitmuster, das ausgegebendes LED-Muster repr�sentiert (1 f�r LED an)
	 */
	uint8_t getLedPattern();
};

#endif

/**
*	\brief	Digitaler Eingang
*/
class DigitalIn
{
protected:
	volatile port_t & iPin;
	const port_t iMask;

public:

	/**	Konstruktor.
	 *	@param pin	Register ber das der Port auslesbar ist (PINx).
	 *	@param port Referenz auf "Inupt-Modus-Register" des Ports (PORTx)
	 *	@param ddr	Referenz auf Data Direction Register des Ports (DDRx)
	 *	@param mask	Bitmaske, ber die der Button angesprochen werden soll
	 */
	DigitalIn(volatile port_t & pin, volatile port_t & port, volatile port_t & ddr, const port_t mask);
	
	/**	Liefert den nach Bitmaske gefilterten Port-Status zurck.
	 */
	inline port_t getStatus()
	{
		return iPin & iMask;
	}
	
};


#ifdef USE_BUTTON

/**
 *	\brief	Einzelner Button
 */
class Button : protected DigitalIn
{
protected:
	bool lastButtonStatus;
	
public:

	/**	Konstruktor.
	 *	@param pin	Register ber das der Port auslesbar ist (PINx).
	 *	@param port "Inupt-Modus-Register" des Ports (PORTx)
	 *	@param ddr	Data Direction Register des Ports (DDRx)
	 *	@param mask	Bitmaske, ber die der Button angesprochen werden soll
	 */
	Button(volatile port_t & pin, volatile port_t & port, volatile port_t & ddr, const port_t mask);

	/** Gibt true zur�ck, wenn der Button grad gedr�ckt ist.
	 */
	bool isPressed();

	/** Gibt true zur�ck, wenn Button neu gedr�ckt wurde.
	 *	Neu gedr�ckt hei&szligt hier: der letzte Aufruf von gotPressed lieferte
	 *	false und isPressed() liefert momentan true.
	 */
	bool gotPressed();
};

#endif

#ifdef USE_BUTTONBLOCK

/**
*	\brief	Block von Buttons (mehrere Buttons an einem Port)
*/
class ButtonBlock : protected DigitalIn
{
protected:
	uint8_t patternShift;
	
public:

	/**	Konstruktor.
	 *	@param pin	Register ber das der Port auslesbar ist (PINx).
	 *	@param port	"Inupt-Modus-Register" des Ports (PORTx)
	 *	@param ddr	Data Direction Register des Ports (DDRx)
	 *	@param mask	Bitmaske, �ber die der Button angesprochen werden soll
	 *	@param shift	Anzahl Bits, um die das pattern nach links geshiftet werden muss, um auf mask zu passen
	 */
	ButtonBlock(volatile port_t & pin, volatile port_t & port, volatile port_t & ddr, const port_t mask, const uint8_t shift);

	/** Gibt Button-Pattern zurck.
	 */
	uint8_t getPressedPattern();

};

#endif
