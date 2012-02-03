#pragma once

#include "timerCommon.h"
#include <avr-halib/interrupts/at90can128/timer0.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace at90can128
{

/** \brief Regmap for asynchronous timer 0 **/
class Timer0 : public base::LocalRegMap, public helpers::CommonTimerDefinitions
{
public:
	enum Parameters
	{
		asyncCapability=false,
		numOCU=1,
		numPS=7
	};
	
	/** \brief supported wave form generation modes **/
	enum WaveForms
	{
		normal              =0,	/**< normal output, no reset **/
		phaseCorrectPWM8,		/**< phase correct pwm output **/
		ctc,					/**< clear timer on compare match **/
		fastPWM8,				/**< fast pwm output **/
	};
	
	/** \brief clock prescaler for this timer **/
	enum Prescalers
	{
		noClock=0,			/**< clock input disabled, no counting **/
		ps1,				/**< \f$ f_{t}=f_{cpu} \f$ **/
		ps8,				/**< \f$ f_{t}=\frac{f_{cpu}}{8} \f$ **/
		ps64,				/**< \f$ f_{t}=\frac{f_{cpu}}{64} \f$ **/
		ps256,				/**< \f$ f_{t}=\frac{f_{cpu}}{256} \f$ **/
		ps1024,				/**< \f$ f_{t}=\frac{f_{cpu}}{1024} \f$ **/
		extClkFalling,		/**< external clock input on falling edge **/
		extClkRising		/**< external clock input on rising edge **/
	};
	
	template<uint8_t i>
	struct PSValue
	{
		static const uint16_t value=(i==ps1)?1:
										(i==ps8)?8:
											(i==ps64)?64:
												(i==ps256)?256:
													1024;
	};

	template<uint8_t i>
	struct PSArray
	{
		static const uint16_t value=PSValue<ps1+i>::value;

		static const Prescalers code=(i==0)?ps1:
										(i==1)?ps8:
											(i==2)?ps64:
												(i==3)?ps256:
													ps1024;
	};
	
	typedef interrupts::at90can128::Timer0IntMap InterruptMap;
	
	typedef uint8_t ValueType;
	
	union
	{
		struct
		{
			uint8_t __base[0x24];
			uint8_t            : 7;
			uint8_t ocmAOutput : 1;
		};
		struct
		{
			uint8_t __pad0[0x35];
			union
			{
				struct
				{
					uint8_t tov : 1;
					uint8_t ocfa : 1;
				};
				uint8_t tifr;
			};
		};
		struct
		{
			uint8_t __pad1[0x6e];
			union
			{
				struct
				{
					uint8_t toie : 1;
					uint8_t ociea : 1;
				};
				uint8_t timsk;
			};
		};
		struct
		{
			uint8_t __pad2[0x44];
			union
			{
				struct
				{
					uint8_t cs   : 3;
					uint8_t wgm0 : 1;
					uint8_t coma : 2;
					uint8_t wgm1 : 1;
					uint8_t foca : 1;
				};
				struct
				{
					uint8_t tccra;
				};
				uint16_t tccr;
			};
		};
		struct
		{
			uint8_t __pad3[0x46];
			uint8_t tcnt;
		};
		struct
		{
			uint8_t __pad4[0x47];
			uint8_t ocra;
		};
	};
	
	/** \brief set wave form
	 * 
	 * this method is needed, since the bits of the wave form generation mode
	 * selection are not consecutive, therefore an accessor method is needed.
	 *
	 * \param form the wave form generation mode to select
	 **/
	void setWaveForm(WaveForms form)
	{
		wgm0=form&0x1;
		wgm1=(form>>1)&0x1;
	}
	
	/** \brief get wave form
	 * 
	 * this method is needed, since the bits of the wave form generation mode
	 * selection are not consecutive, therefore an accessor method is needed.
	 *
	 * \return the wave form generation mode selected
	 **/
	WaveForms getWaveForm() const
	{
		return (WaveForms)(wgm0|(wgm1<<1));
	}
}__attribute__((packed));
}
}
}
}
