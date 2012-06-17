#pragma once

#include "timer.h"

#include <stdint.h>

namespace avr_halib{
namespace config{
namespace timer
{
	template<PrescalerType ps>
	struct PrescalerToValue;

	template<uint8_t value>
	struct ValueToPrescaler;

	template<>
	struct PrescalerToValue<Prescalers::ps1>
	{
		static const uint8_t value = 1;
	};

	template<>
	struct PrescalerToValue<Prescalers::ps8>
	{
		static const uint8_t value = 8;
	};

	template<>
	struct PrescalerToValue<Prescalers::ps32>
	{
		static const uint8_t value = 32;
	};

	template<>
	struct PrescalerToValue<Prescalers::ps64>
	{
		static const uint8_t value = 64;
	};

	template<>
	struct PrescalerToValue<Prescalers::ps128>
	{
		static const uint8_t value = 128;
	};

	template<>
	struct PrescalerToValue<Prescalers::ps256>
	{
		static const uint8_t value = 256;
	};

	template<>
	struct PrescalerToValue<Prescalers::ps1024>
	{
		static const uint8_t value = 1024;
	};

	template<>
	struct ValueToPrescaler<1>
	{
		static const PrescalerType ps = Prescalers::ps1;
	};

	template<>
	struct ValueToPrescaler<8>
	{
		static const PrescalerType ps = Prescalers::ps8;
	};

	template<>
	struct ValueToPrescaler<32>
	{
		static const PrescalerType ps = Prescalers::ps32;
	};	

	template<>
	struct ValueToPrescaler<64>
	{
		static const PrescalerType ps = Prescalers::ps64;
	};

	template<>
	struct ValueToPrescaler<128>
	{
		static const PrescalerType ps = Prescalers::ps128;
	};

	template<>
	struct ValueToPrescaler<256>
	{
		static const PrescalerType ps = Prescalers::ps256;
	};

	template<>
	struct ValueToPrescaler<1024>
	{
		static const PrescalerType ps = Prescalers::ps1024;
	};
}
}
}
