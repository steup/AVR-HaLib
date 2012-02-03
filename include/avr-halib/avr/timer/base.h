#pragma once

#include <avr-halib/avr/timer/halSelector.h>

namespace avr_halib
{
namespace drivers
{
namespace timer
{
	template<typename config>
	class Base : public HALSelector<config>::type
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::WaveForms WaveForms;
			typedef typename RegMap::ValueType ValueType;

		public:
			Base()
			{
				UseRegMap(rm, RegMap);
				rm.tcnt  = 0;
				rm.tifr  = 0;
				rm.cs    = RegMap::noClock;
				rm.toie  = config::overflowInt;
				rm.setWaveForm(config::waveForm);

			}

			void setWaveForm(WaveForms newWaveForm)
			{
				UseRegMap(rm, RegMap);
				rm.setWaveForm(newWaveForm);
				SyncRegMap(rm);
				this->sync();
			}

			WaveForms getWaveForm() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.getWaveForm();
			}

			void setCounter(ValueType value)
			{
				UseRegMap(rm, RegMap);
				rm.tcnt=value;
				SyncRegMap(rm);
				this->sync();
			}

			ValueType getCounter() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.tcnt;
			}

			static void setOverflowInt(bool value)
			{
				UseRegMap(rm, RegMap);
				rm.toie=value;
				SyncRegMap(rm);
			}

			bool getOverflowInt() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.toie;
			}

			void start()
			{
				UseRegMap(rm, RegMap);
				rm.cs=config::ps;
				SyncRegMap(rm);
				this->sync();
			}

			void stop()
			{
				UseRegMap(rm, RegMap);
				rm.cs=RegMap::noClock;
				SyncRegMap(rm);
				this->sync();
			}

/*			template<typename T, void (T::*F)(void)>
			void registerOverflowCallback(T& obj)
			{
				interrupts::Interrupt<IntMap>::template setInt<IntMap::overflow_Int, T, F>(obj);
			}

			template<typename T, void (T::*F)(void)>
			void registerOverflowCallback(const T& obj)
			{
				interrupts::Interrupt<IntMap>::template setInt<IntMap::overflow_Int, T, F>(obj);
			}

			template<typename T, void (*F)(void)>
			void registerOverflowCallback()
			{
				interrupts::Interrupt<IntMap>::template setInt<IntMap::overflow_Int, F>();
			}*/
	};
}
}
}
