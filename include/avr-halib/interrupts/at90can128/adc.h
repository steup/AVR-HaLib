#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Adc
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			conversionComplete=25,	/**< conversion complete interrupt **/
		};

        typedef ::Interrupt::Slot< conversionComplete, 
                                   ::Interrupt::Binding::DynamicPlainFunction 
                                 > ConversionCompleteSlot;

		typedef boost::mpl::vector< ConversionCompleteSlot >::type Slots;
	};
}

}
}
