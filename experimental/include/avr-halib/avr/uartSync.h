#pragma once

#include <avr-halib/share/delay.h>

namespace avr_halib
{
namespace power
{
	template<typename RegMap>
	struct UartSleepSynchronizer
	{
		void sync()
		{
			delay_us(12*1000000ULL/RegMap::baudrate);
		}
	};
}
}
