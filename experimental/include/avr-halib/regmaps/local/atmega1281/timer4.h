#pragma once

#include <avr-halib/regmaps/local/atmega1281/genericTimer.h>
#include <avr-halib/interrupts/atmega1281/timer4.h>

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
	struct Timer4Desc
	{
		typedef interrupts::atmega1281::Timer4IntMap IntMap;

		enum Addresses
		{
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
