/**
 *	\file	uc.cpp
 *	\brief	Target microcontroller code (microcontroller definitions and functions for using them)
 *	\author	Philipp Werner, Karl Fessel (Port2Mem for atmega128)
 *	\date	20.05.2008
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */


#include "uc.h"
#include "parse.h"

#include <string>
#include <sstream>
#include <ctype.h>


namespace uc
{

Port2Mem p2m_atmega32 [] =
{
	{ 'd',	PIN,	0x30 },
	{ 'd',	DDR,	0x31 },
	{ 'd',	PORT,	0x32 },
	{ 'c',	PIN,	0x33 },
	{ 'c',	DDR,	0x34 },
	{ 'c',	PORT,	0x35 },
	{ 'b',	PIN,	0x36 },
	{ 'b',	DDR,	0x37 },
	{ 'b',	PORT,	0x38 },
	{ 'a',	PIN,	0x39 },
	{ 'a',	DDR,	0x3a },
	{ 'a',	PORT,	0x3b },
	{ 0, NONE, 0 }
};

AvrUC uc_atmega32 =
{
	"atmega32",
	p2m_atmega32,
	"a..d"
};

Port2Mem p2m_atmega128 [] =
{
	{ 'g',	PIN,	0x63 },
	{ 'g',	DDR,	0x64 },
	{ 'g',	PORT,	0x65 },
	
	{ 'f',	PIN,	0x20 }, //ja wirklich
	{ 'f',	DDR,	0x61 },
	{ 'f',	PORT,	0x62 },
	
 	{ 'e',	PIN,	0x21 },
	{ 'e',	DDR,	0x22 },
	{ 'e',	PORT,	0x23 },
	
	{ 'd',	PIN,	0x30 },
	{ 'd',	DDR,	0x31 },
	{ 'd',	PORT,	0x32 },
	
	{ 'c',	PIN,	0x33 },
	{ 'c',	DDR,	0x34 },
	{ 'c',	PORT,	0x35 },
	
	{ 'b',	PIN,	0x36 },
	{ 'b',	DDR,	0x37 },
	{ 'b',	PORT,	0x38 },
	
	{ 'a',	PIN,	0x39 },
	{ 'a',	DDR,	0x3a },
	{ 'a',	PORT,	0x3b },
	{ 0, NONE, 0 }
};

AvrUC uc_atmega128 =
{
	"atmega128",
	p2m_atmega128,
	"a..g"
};

Port2Mem p2m_atmega1281 [] =
{
	{ 'a',	PIN,	0x20 },
	{ 'a',	DDR,	0x21 },
	{ 'a',	PORT,	0x22 },
	{ 'b',	PIN,	0x23 },
	{ 'b',	DDR,	0x24 },
	{ 'b',	PORT,	0x25 },
	{ 'c',	PIN,	0x26 },
	{ 'c',	DDR,	0x27 },
	{ 'c',	PORT,	0x28 },
	{ 'd',	PIN,	0x29 },
	{ 'd',	DDR,	0x2a },
	{ 'd',	PORT,	0x2b },
	{ 'e',	PIN,	0x2c },
	{ 'e',	DDR,	0x2d },
	{ 'e',	PORT,	0x2e },
	{ 'f',	PIN,	0x2f },
	{ 'f',	DDR,	0x30 },
	{ 'f',	PORT,	0x31 },
	{ 'g',	PIN,	0x32 },
	{ 'g',	DDR,	0x33 },
	{ 'g',	PORT,	0x34 },
	{ 0, NONE, 0 }
};


AvrUC uc_atmega1281 =
{
	"atmega1281",
	p2m_atmega1281,
	"a..g"
};

AvrUC uc_atmega128rfa1 =
{
	"atmega128rfa1",
	p2m_atmega1281, //wie beim 1281
	"a..g"
};


Port2Mem p2m_at90can128 [] =
{
	{ 'a',	PIN,	0x20 },
	{ 'a',	DDR,	0x21 },
	{ 'a',	PORT,	0x22 },
	{ 'b',	PIN,	0x23 },
	{ 'b',	DDR,	0x24 },
	{ 'b',	PORT,	0x25 },
	{ 'c',	PIN,	0x26 },
	{ 'c',	DDR,	0x27 },
	{ 'c',	PORT,	0x28 },
	{ 'd',	PIN,	0x29 },
	{ 'd',	DDR,	0x2a },
	{ 'd',	PORT,	0x2b },
	{ 'e',	PIN,	0x2c },
	{ 'e',	DDR,	0x2d },
	{ 'e',	PORT,	0x2e },
	{ 'f',	PIN,	0x2f },
	{ 'f',	DDR,	0x30 },
	{ 'f',	PORT,	0x31 },
	{ 'g',	PIN,	0x32 },
	{ 'g',	DDR,	0x33 },
	{ 'g',	PORT,	0x34 },
	{ 0, NONE, 0 }
};


AvrUC uc_at90can128 =
{
	"at90can128",
	p2m_at90can128,
	"a..f"
};



const AvrUC * getTargetController(std::string s)
{
	for (int i = s.length() - 1; i >= 0; i--)
		s[i] = tolower(s[i]);

	if (s == uc_at90can128.name)
		return &uc_at90can128;
	else if (s == uc_atmega32.name)
		return &uc_atmega32;
	else if (s == uc_atmega128.name)
		return &uc_atmega128;
	else if (s == uc_atmega1281.name)
		return &uc_atmega1281;
	else if (s == uc_atmega128rfa1.name)
		return &uc_atmega128rfa1;
	else
		return 0;
}

const char * getSupportedControllers()
{
	return "atmega32, at90can128, atmega128, atmega1281, atmega128rfa1";
}



void checkPort(char port, const AvrUC * targetController)
{
	if (!targetController)
		return;

	bool portFound = false;
	for (int i = 0; targetController->p2m[i].portChar; i++)
	{
		if (port == targetController->p2m[i].portChar)
		{
			portFound = true;
			break;
		}
	}
	
	if (!portFound)
	{
		std::ostringstream s;
		s << "no port ’" << port << "’ available on this chip, valid: " << targetController->portRange;
		parse::parseError(s.str(), false);
	}
}

void checkPin(int pin)
{
	if (pin < 0 || pin > 7)
	{
		std::ostringstream s;
		s << "’" << pin << "’ is no valid pin number on this chip, valid: 0..7";
		parse::parseError(s.str(), false);
	}
}

int getAddress(char port, PinType pinType, const AvrUC * targetController)
{
	if (!targetController)
		return -1;

	for (int i = 0; targetController->p2m[i].portChar; i++)
		if (port == targetController->p2m[i].portChar && pinType == targetController->p2m[i].pinType)
			return targetController->p2m[i].address;
	
	return -1;
}

const char * getPinTypeString(PinType pinType)
{
	static const char * s [] = { "NONE", "PIN", "PORT", "DDR" };
	return s[pinType];
}


}	// namespace uc
