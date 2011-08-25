#pragma once

#include <avr-halib/avr/timer/base.h>
#include <avr-halib/avr/timer/dynamicPrescaler.h>
#include <boost/mpl/if.hpp>
#include <avr-halib/avr/timer/interruptConfig.h>

namespace avr_halib
{
namespace drivers
{
namespace timer
{
	using boost::mpl::if_c;

	template<typename config>
	struct Timer : public if_c< config::dynamicPrescaler,
								DynamicPrescaler<config, Base>,
								Base<config>
							>::type
	{
		private:
			typedef typename if_c< config::dynamicPrescaler, 
								   DynamicPrescaler<config, Base>,
								   Base<config>
							   >::type BaseDriver;

		public:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::InterruptMap::Slots InterruptSlotList;
			typedef typename RegMap::InterruptMap InterruptMap;
			typedef typename BaseDriver::OutputCompareUnits OutputCompareUnits;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;
			typedef typename RegMap::WaveForms WaveForms;
			typedef typename config::TimerFrequency InputFrequency; 
	};
}
}
}
