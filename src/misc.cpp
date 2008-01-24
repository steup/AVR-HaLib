/**
 *	\file halib/misc.cpp
 *	\brief	Implementiert Digital-IO, Benutzerschnittstelle (LEDs und Buttons)
 */


#include "halib/config.h"
#include "halib/misc.h"


DigitalOut::DigitalOut(volatile port_t & port, volatile port_t & ddr, const port_t mask) :
	oPort(port), oMask(mask)
{
	ddr  |= mask;		// configure pins for output
	port |= mask;		// init to 1 (e.g. LED off)
}

void DigitalOut::replace(const port_t pattern)
{
	oPort = (pattern & oMask) | (oPort & ~oMask); 
}


#ifdef USE_LED

Led::Led(volatile port_t & port, volatile port_t & ddr, const port_t mask) :
	DigitalOut(port, ddr, mask)
{
}

#endif


#ifdef USE_LEDBLOCK

LedBlock::LedBlock(volatile port_t & port, volatile port_t & ddr, const port_t mask, const uint8_t shift) :
	DigitalOut(port, ddr, mask),
	patternShift(shift)
{
}

void LedBlock::setLedPattern(const uint8_t pattern)
{
#if defined (__AVR_ATmega32__)
	replace((~pattern) << patternShift);
#elif defined (__AVR_AT90CAN128__)
	replace((pattern) << patternShift);
#else
#	warning "defaulting to pos led"
	replace((pattern) << patternShift);
#endif
}
	
uint8_t LedBlock::getLedPattern()
{
	
#if defined (__AVR_ATmega32__)
		return ~getStatus() >> patternShift;
#elif defined (__AVR_AT90CAN128__)
		return getStatus() >> patternShift;
#else
#	warning "defaulting to pos led"
		return getStatus() >> patternShift;
#endif

}

#endif


DigitalIn::DigitalIn(volatile port_t & pin, volatile port_t & port, volatile port_t & ddr, const port_t mask) :
	iPin(pin), iMask(mask)
{
	ddr  &= ~mask;		// set to 0 (Input)
	port |= mask;		// Set pullups (Bit auf 1)
}


#ifdef USE_BUTTON

Button::Button(volatile port_t & pin, volatile port_t & port, volatile port_t & ddr, const port_t mask) :
	DigitalIn(pin, port, ddr, mask),
	lastButtonStatus(false)
{
}

bool Button::isPressed()
{
	return getStatus() == 0;
}

bool Button::gotPressed()
{
	if (!lastButtonStatus)
	{
		if (isPressed())
		{
			// Button gerade gedrckt und zuletzt nicht gedrckt gewesen...
			lastButtonStatus = true;
			return true;
		}
		return false;
	}
	else
	{
		if (!isPressed())
		{
			// Button nicht mehr gedrckt aber zuletzt gedrckt gewesen...
			lastButtonStatus = false;
		}
		return false;
	}
}

#endif


#ifdef USE_BUTTONBLOCK

ButtonBlock::ButtonBlock(volatile port_t & pin, volatile port_t & port, volatile port_t & ddr, const port_t mask, const uint8_t shift) :
	DigitalIn(pin, port, ddr, mask),
	patternShift(shift)
{
}

uint8_t ButtonBlock::getPressedPattern()
{
	return ((~iPin) & iMask) >> patternShift;
}

#endif
