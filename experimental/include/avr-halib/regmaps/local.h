#pragma once

namespace avr_halib
{
namespace regmaps
{
namespace local
{

#ifdef __AVR_AT90CAN128__
	#include "local/at90can128.h"
#else
	#error "MCU not supported yet"
#endif

}
}
}
