#pragma once

#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Spi
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			operationComplete = 20,	/**< operation complete **/
		};

        typedef ::Interrupt::Slot< operationComplete, 
                                   ::Interrupt::Binding::DynamicPlainFunction 
                                 > OperationCompleteSlot;

		typedef boost::mpl::vector< OperationCompleteSlot >::type Slots;
	};
}

}
}

