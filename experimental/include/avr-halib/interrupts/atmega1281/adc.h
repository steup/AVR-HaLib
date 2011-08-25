#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct ADCIntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			conversionComplete=29,	/**< conversion complete interrupt **/
		};

		typedef helpers::Slot<conversionComplete> ConversionCompleteSlot;
		typedef typename boost::mpl::vector<ConversionCompleteSlot>::type Slots;
	};

	
}

template<>
struct InterruptRegistration<atmega1281::ADCIntMap, false>
{
	private:
		typedef atmega1281::ADCIntMap::Interrupts Int;

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}

	template<Int i, void (*F)(void)>
	static void setInt()
	{
		redirectISRF(ADC_vect, F);
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(const T& obj)
	{
		redirectISRM(ADC_vect, F, obj);
	}
};

}
}
