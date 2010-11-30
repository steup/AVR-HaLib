#pragma once

namespace avr_halib
{
namespace drivers
{
namespace timer
{
	template<typename config, template<typename> class BaseDriver>
	struct DynamicPrescaler : public BaseDriver<config>
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::Prescalers Prescalers;
			
			Prescalers ps;

		public:
			DynamicPrescaler()
			{
				ps=config::ps;
			}

			void setPrescaler(Prescalers newPs)
			{
				ps=newPs;
				start();
			}

			Prescalers getPrescaler() const
			{
				return ps;
			}

			void start()
			{
				UseRegMap(rm, RegMap);
				rm.cs=ps;
				SyncRegMap(rm);
				this->sync();
			}
	};
}
}
}
