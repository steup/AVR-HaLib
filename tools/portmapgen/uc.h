/**
 *	\file	uc.h
 *	\brief	Target microcontroller code
 *	\author	Philipp Werner
 *	\date	20.05.2008
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include <string>

namespace uc
{

enum PinType { NONE = 0, PIN = 1, PORT = 2, DDR = 3 };

struct Port2Mem
{
	char	portChar;
	PinType	pinType;
	int	address;
};

struct AvrUC
{
	const char * name;		// lower case!
	Port2Mem * p2m;
	const char * portRange; 
};

const AvrUC * getTargetController(std::string s);
const char * getSupportedControllers();

void checkPort(char port, const AvrUC * targetController);
void checkPin(int pin);

int getAddress(char port, PinType pinType, const AvrUC * targetController);

const char * getPinTypeString(PinType pinType);

}
