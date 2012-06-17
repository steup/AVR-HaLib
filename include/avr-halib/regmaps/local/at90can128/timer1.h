#pragma once

#include "generalTimer.h"
#include <avr-halib/interrupts/at90can128/timer1.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace at90can128 
{
	namespace timerDescription
	{
		struct Timer1
		{
			typedef interrupts::at90can128::Timer1IntMap InterruptMap;

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
		
	typedef generalTimer::16Bit< timerDescriptions::Timer1 > Timer1;
}
}
}
}
