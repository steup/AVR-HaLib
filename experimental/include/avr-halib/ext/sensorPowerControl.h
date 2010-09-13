#pragma once

#include <avr-halib/avr/portmap.h>

namespace avr_halib
{
namespace drivers
{
/**\brief namespace containing drivers for robbyboard specific features**/
namespace robby
{

/** \brief Driver for the power source of the ADC sensors of the EOS robbyboard
 * 	\tparam RobbyADCPowerPortMap The Port Map to access the adc power pins on the robby board.
 *
 * 	This class provides power supply control for ADC sensors on the EOS robbyboard.
 **/
template<typename PM>
class SensorPowerControl
{
	private:
		/**\brief Make PortMap accessible for driver users**/
		typedef PM PortMap;

	public:
		/** \brief Constructor
		 * 	
		 * 	Configures ports and deactivates the power source of all sensors
		 **/
		SensorPowerControl()
		{
			UsePortmap(sps, PM);
			sps.outPort.setDdr(0xFF);
			sps.outPort.setPort(0xFF);
			SyncPortmap(sps);
		}

	public:
		/** \brief activate all power supplies **/
		void setActive()
		{
			UsePortmap(sps, PM);
			sps.outPort.setPort(0x00);
			SyncPortmap(sps);
		}

		/** \brief activate one power supply
		 *  \param i the number of the power supply
		 **/
		void setActive(uint8_t i)
		{
			UsePortmap(sps, PM);
			uint8_t port=sps.outPort.getPort();
			sps.outPort.setPort(~(0x1<<i)&port);
			SyncPortmap(sps);
		}
		
		/** \brief deactivate all power supplies **/
		void setInactive()
		{
			UsePortmap(sps, PM);
			sps.outPort.setPort(0xFF);
			SyncPortmap(sps);
		}

		/** \brief activate one power supply
		 *  \param i the number of the power supply
		 **/
		void setInactive(uint8_t i)
		{
			UsePortmap(sps, PM);
			uint8_t port=sps.outPort.getPort();
			sps.outPort.setPort((0x1<<i)|port);
			SyncPortmap(sps);
		}
};
}
}
}
