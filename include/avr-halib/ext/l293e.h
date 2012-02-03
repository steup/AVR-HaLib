#pragma once

namespace avr_halib
{
namespace drivers
{
namespace external
{

	template<typename PortMap, bool inverted=false>
	class L293E
	{
			static const uint8_t ddrMask=0x3;
		public:
			enum States
			{
				freeRunning=0,
				forward=(inverted?2:1),
				backward=(inverted?1:2),
				halt=3
			};

			L293E()
			{
				UsePortmap(pm, PortMap);
				pm.state.setDdr(ddrMask);
				SyncPortmap(pm);
			}

			void state(States state)
			{
				UsePortmap(pm, PortMap);
				pm.state.setPort(state);
				SyncPortmap(pm);
			}

			States state() const
			{
				UsePortmap(pm, PortMap);
				SyncPortmap(pm);
				return (States)pm.state.getPort();
			}
	};
}
}
}
