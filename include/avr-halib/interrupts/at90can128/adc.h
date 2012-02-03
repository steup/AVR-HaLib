#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <boost/mpl/vector.hpp>

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

		typedef helpers::Slot<conversionComplete> ConversionCompleteSlot;
		typedef boost::mpl::vector<ConversionCompleteSlot>::type Slots;
	};
}

template<>
struct InterruptRegistration<at90can128::ADCIntMap, false>
{
	private:
		typedef at90can128::ADCIntMap::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(ADC_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}
};
}
}
