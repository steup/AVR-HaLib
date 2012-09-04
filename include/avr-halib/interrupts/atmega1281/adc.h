#pragma once

#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Adc
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			conversionComplete = 29,	/**< conversion complete interrupt **/
		};

        typedef ::Interrupt::Slot< conversionComplete, 
                                   ::Interrupt::Binding::DynamicPlainFunction 
                                 > ConversionCompleteSlot;

		typedef boost::mpl::vector< ConversionCompleteSlot >::type Slots;
	};
}

}
}
