#pragma once

#include "genericTimer.h"
#include <avr-halib/interrupts/atmega128rfa1/timer1.h>

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
	struct Timer1Desc
	{
		typedef interrupts::atmega128rfa1::Timer1 InterruptMap;

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
