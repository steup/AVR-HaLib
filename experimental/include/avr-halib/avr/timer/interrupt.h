#pragma once

#include <avr-halib/interrupts/interrupt.h>

namespace avr_halib
{
namespace drivers
{
namespace timer
{
	template<typename IntMap>
	struct Interrupts
	{
		template<typename IntMap::Interrupts i, typename T, void (T::*F)(void)>
		void registerCallback(T& obj)
		{
			interrupts::Interrupt<IntMap>::template setInt<i, T, F>(obj);
		}
	};
}
}
}
