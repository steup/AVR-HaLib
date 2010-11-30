#pragma once

#include <avr-halib/regmaps/regmaps.h>

#ifdef __AVR_AT90CAN128__
	#include "local/at90can128.h"
#else
	#error "MCU not supported yet"
#endif
