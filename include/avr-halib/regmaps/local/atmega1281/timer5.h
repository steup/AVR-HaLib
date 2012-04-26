#pragma once

#include "genericTimer.h"
#include <avr-halib/interrupts/atmega128rfa1/timer5.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{
namespace helpers
{
	struct Timer5Desc
	{
		typedef interrupts::atmega128rfa1::Timer5 InterruptMap;

		enum Addresses
		{
			ocmOutput=0x10A,
			ocmOffset=3,
			tifr=0x3A,
			timsk=0x73,
			tccr=0x120,
			tcnt=0x124,
			icr=0x126,
			ocr=0x128,
		};
	};
}

	typedef helpers::GenericTimer<helpers::Timer5Desc> Timer5;
}
}
}
}

