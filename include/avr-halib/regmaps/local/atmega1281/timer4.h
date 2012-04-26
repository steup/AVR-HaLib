#pragma once

#include "genericTimer.h"
#include <avr-halib/interrupts/atmega128rfa1/timer4.h>

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
	struct Timer4Desc
	{
		typedef interrupts::atmega128rfa1::Timer4 InterruptMap;

		enum Addresses
		{
			ocmOutput=0x101,
			ocmOffset=3,
			tifr=0x39,
			timsk=0x72,
			tccr=0xA0,
			tcnt=0xA4,
			icr=0xA6,
			ocr=0xA8,
		};
	};
}

	typedef helpers::GenericTimer<helpers::Timer4Desc> Timer4;
}
}
}
}

