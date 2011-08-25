#pragma once

#include <avr-halib/regmaps/local/at90can128/genericTimer.h>
#include <avr-halib/interrupts/at90can128/timer3.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace at90can128
{
namespace helpers
{
	struct Timer3Desc
	{
		typedef interrupts::at90can128::Timer3IntMap InterruptMap;

		enum Addresses
		{
			ocmOutput=0x2D,
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

