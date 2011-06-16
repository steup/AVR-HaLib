#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct ADCIntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			conversionComplete=25,	/**< conversion complete interrupt **/
		};
	};
}

template<>
struct Interrupt<at90can128::ADCIntMap>
{
	private:
		typedef at90can128::ADCIntMap::Interrupts IntType;

	public:
	template<IntType i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}

	template<IntType i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}

	template<IntType i, void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(ADC_vect, F);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(ADC_vect, F);
	}
};

}
}
