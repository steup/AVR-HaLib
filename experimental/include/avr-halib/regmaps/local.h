#pragma once

#include <avr-halib/regmaps/regmaps.h>

#if defined(__AVR_AT90CAN128__)
	#include "local/at90can128.h"
#elif defined(__AVR_ATmega1281__)
	#include "local/atmega1281.h"
#elif defined(__AVR_ATmega128RFA1__)
	#include "local/atmega128rfa1.h"
#else
	#error "MCU not supported yet"
#endif
