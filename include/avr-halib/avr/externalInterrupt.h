#pragma once

#include <avr-halib/regmaps/local.h>

namespace avr_halib
{
namespace drivers
{
	namespace helpers
	{
		template<typename ExtIntRegMap, typename ExtIntRegMap::Types type>
		class ExtIntBase : public interrupts::InterruptRegistration<
						   			typename ExtIntRegMap::InterruptMap >
		{
			public:
				typedef typename ExtIntRegMap::InterruptMap InterruptMap;
				typedef typename InterruptMap::Slots InterruptSlotList;

			protected:

				typedef ExtIntRegMap RegMap;

				ExtIntBase()
				{
					UseRegMap(rm, RegMap);
					rm.flag =false;
					rm.mask =true;
					rm.type =type;
					SyncRegMap(rm);
				}

			public:
				bool getState() const
				{
					UseRegMap(rm, RegMap);
					SyncRegMap(rm);
					return rm.flag;
				}

				void enable()
				{
					UseRegMap(rm, RegMap);
					rm.mask =true;
					SyncRegMap(rm);
				}

				void disable()
				{
					UseRegMap(rm, RegMap);
					rm.mask =false;
					SyncRegMap(rm);
				}
		};
	}

	template<typename ExtIntRegMap, typename ExtIntRegMap::Sensitivity sens >
	class ExternalInterrupt 
		: public helpers::ExtIntBase<ExtIntRegMap, ExtIntRegMap::external >
	{
			typedef ExtIntRegMap RegMap;

		public:
			ExternalInterrupt()
			{
				this->setSensitivity(sens);
			}

			void setSensitivity(typename RegMap::Sensitivity value)
			{
				UseRegMap(rm, RegMap);
				rm.sensitivity =value;
				SyncRegMap(rm);
			}

			void setPullUp(bool value)
			{
				UseRegMap(rm, RegMap);
				rm.trigger =value;
				SyncRegMap(rm);
			}
	};

	template<typename ExtIntRegMap>
	class SoftwareInterrupt 
		: public helpers::ExtIntBase<ExtIntRegMap, ExtIntRegMap::software >
	{
			typedef ExtIntRegMap RegMap;

		public:
			SoftwareInterrupt()
			{
				this->setSensitivity(RegMap::rising);
			}

			void trigger()
			{
				UseRegMap(rm, RegMap);
				rm.trigger =true;
				SyncRegMap(rm);
				rm.trigger =false;
				SyncRegMap(rm);
			}
	};

}
}
