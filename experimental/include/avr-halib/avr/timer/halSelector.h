#pragma once

#include <avr-halib/avr/timer/outputCompareUnit.h>
#include <avr-halib/avr/timer/asyncHandler.h>
#include <avr-halib/interrupts/interrupt.h>

namespace avr_halib
{
namespace drivers
{
namespace timer
{
	template<typename config>
	struct HALSelector
	{
		enum Parameters
		{
			enableAsync=config::async && config::RegMap::asyncCapability,
			numOCU=config::RegMap::numOCU
		};

		typedef AsyncHandler<config, enableAsync> Async;
		typedef OutputCompareUnit<config, enableAsync, numOCU> OCUs;
		typedef typename config::RegMap::IntMap IntMap;
		typedef interrupts::Interrupt<IntMap> Ints;
		struct Temp{
			struct Interrupts : public IntMap{};
		};

		struct type : public Async, public OCUs, public Ints, public Temp{};
	};
}
}
}
