#pragma once

#include <avr-halib/avr/externalInterrupt.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <avr-halib/share/fixPoint.h>

namespace avr_halib
{
namespace drivers
{
namespace external
{
	template<typename config>
	class OdometrieSensor : private ExternalInterrupt< typename config::TickSource, 
													   config::TickSource::rising
													 >
	{
			typedef ExternalInterrupt< typename config::TickSource, config::TickSource::rising > TickSource;

		public:
			typedef typename config::EvalFrequency EvalFrequency;
			typedef typename EvalFrequency::template mult<Frequency<60, config::ticksPerTurn> >::type UMinFreq;

			typedef typename config::CounterType CounterType;

		private:

			volatile CounterType count;
			volatile CounterType odoValue;

			typedef typename config::EvalDelegate EvalDelegate;

			void tick()
			{
				count++;
				PINB|=1;
			}

		public:

			typedef typename TickSource::InterruptMap InterruptMap;
			typedef typename TickSource::InterruptSlotList InterruptSlotList;

			OdometrieSensor() : count(0), odoValue(0)
			{
				DDRB|=1;
				this->setPullUp(true);
				InterruptMap::ExternalInterruptSlot::template bind<OdometrieSensor, &OdometrieSensor::tick>(this);
			}

			void eval()
			{
				odoValue=count;
				count=0;
			}

			uint16_t getValue() const
			{
				typedef object::FixPointValue<uint32_t, 8> CalcType;
				CalcType temp=odoValue;
				temp*=(UMinFreq::numerator*256/UMinFreq::denominator);
				temp/=256;
				return temp.trunc();
			}
	};
}
}
}
