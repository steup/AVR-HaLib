#pragma once

#include <avr-halib/regmaps/local/atmega1281/genericTimer.h>
#include <avr-halib/interrupts/atmega1281/timer1.h>

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
	struct Timer1Desc
	{
		typedef interrupts::atmega1281::Timer1IntMap InterruptMap;

		enum Addresses
		{
			ocmOutput=0x24,
			ocmOffset=5,
			tifr=0x36,
			timsk=0x6F,
			tccr=0x80,
			tcnt=0x84,
			icr=0x86,
			ocr=0x88,
		};
	};
}
		
	typedef helpers::GenericTimer<helpers::Timer1Desc> Timer1;
}
}
}
}
