#pragma once

#include <avr-halib/common/fixPoint.h>

namespace avr_halib
{
namespace object
{
	template<typename config>
	class PIDControl
	{
		public:
			typedef typename config::BaseType InputType;
			static const InputType kp=config::kp;
			static const InputType kd=config::kd;
			static const InputType ki=config::ki;
			static const InputType min=config::min;
			static const InputType max=config::max;

			typedef FixPointValue<typename config::BaseType, config::exponent> CalcType;

		private:
			static const uint8_t windowSize=8;
			CalcType e[windowSize];
			uint8_t index;
			CalcType sumE;
			CalcType targetValue;
			CalcType currentValue;

		public:

			PIDControl() : index(0), sumE(0), targetValue(0), currentValue(0)
			{
				for(uint8_t i=0;i<windowSize;i++)
					e[i]=0;
			}

			void target(InputType target)
			{
				this->targetValue=CalcType(1)*target;
			}

			InputType target() const
			{
				return targetValue.trunc();
			}

			const CalcType& current() const
			{
				return currentValue;
			}

			InputType control(InputType input)
			{
				CalcType calcKp=CalcType::epsilon()*this->kp;
				CalcType calcKi=CalcType::epsilon()*this->ki;
				CalcType calcKd=CalcType::epsilon()*this->kd;
				CalcType newE=targetValue-input;
			
				CalcType oldE=e[index];

				CalcType dE= newE - oldE;

				if(index >= windowSize)
					index=0;
				else
					index++;
				
				sumE+=newE-e[index];

				e[index]=newE;

				currentValue+=(newE * calcKp + dE * calcKd + sumE * calcKi / windowSize);

				if(currentValue > CalcType(max))
					currentValue = CalcType(max);
				if(currentValue < CalcType(min))
					currentValue = CalcType(min);

				return currentValue.trunc();
			}
	};
}
}
