#pragma once

#include <avr-halib/regmaps/local/atmega1281/genericTimer.h>
#include <avr-halib/interrupts/atmega1281/timer3.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega1281
{
namespace helpers
{
	struct Timer3Desc
	{
		typedef interrupts::atmega1281::Timer3IntMap InterruptMap;

		enum Addresses
		{
			ocmOutput=0x2E,
			ocmOffset=3,
			tifr=0x38,
			timsk=0x71,
			tccr=0x90,
			tcnt=0x94,
			icr=0x96,
			ocr=0x98,
		};
	};
}

	typedef helpers::GenericTimer<helpers::Timer3Desc> Timer3;
}
}
}
}

