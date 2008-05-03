/**
 *	\brief	Target microcontroller code
 *	\author	Philipp Werner
 *	\date	22.04.2008
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

void checkPort(const char port, const AvrUC * targetController);
void checkPin(int pin);

}
