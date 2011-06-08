#pragma once

#include "timerCommon.h"
#include <avr-halib/interrupts/atmega128rfa1/timer2.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{

/** \brief Regmap for asynchronous timer 2 **/
class Timer2 : public base::LocalRegMap, public helpers::CommonTimerDefinitions
{
public:
	enum Parameters
	{
		asyncCapability=true,
		numOCU=2,
		numPS=7
	};
	
	/** \brief supported wave form generation modes **/
	enum WaveForms
	{
		normal              =0,	/**< normal output, no reset **/
		phaseCorrectPWM,		/**< phase correct pwm output **/
		ctc,					/**< clear timer on compare match **/
		fastPWM,				/**< fast pwm output **/
		phaseCorrectPWMOCRA =5,	/**< phase correct pwm output **/
		fastPWMOCRA         =7	/**< fast pwm output **/
		
		
	};
	
	/** \brief clock prescaler for this timer **/
	enum Prescalers
	{
		noClock=0,			/**< clock input disabled, no counting **/
		ps1,				/**< \f$ f_{t}=f_{cpu} \f$ **/
		ps8,				/**< \f$ f_{t}=\frac{f_{cpu}}{8} \f$ **/
		ps32,				/**< \f$ f_{t}=\frac{f_{cpu}}{32} \f$ **/
		ps64,				/**< \f$ f_{t}=\frac{f_{cpu}}{64} \f$ **/
		ps128,				/**< \f$ f_{t}=\frac{f_{cpu}}{128} \f$ **/
		ps256,				/**< \f$ f_{t}=\frac{f_{cpu}}{256} \f$ **/
		ps1024				/**< \f$ f_{t}=\frac{f_{cpu}}{1024} \f$ **/
	};
	
	template<uint8_t i>
	struct PSArray
	{
		static const uint16_t value=(i==0)?1:
		(i==1)?8:
		(i==2)?32:
		(i==3)?64:
		(i==4)?128:
		(i==5)?256:
		1024;
		
		static const Prescalers code=(i==0)?ps1:
		(i==1)?ps8:
		(i==2)?ps32:
		(i==3)?ps64:
		(i==4)?ps128:
		(i==5)?ps256:
		ps1024;
	};
	
	typedef interrupts::atmega128rfa1::Timer2IntMap IntMap;
	
	typedef uint8_t ValueType;
	
	union
	{
		struct
		{
		private:
			uint8_t __base[0x37];
		public:
			union
			{
				struct
				{
					uint8_t tov : 1;
					uint8_t ocfa : 1;
					uint8_t ocfb : 1;
				};
				uint8_t tifr;
			};
		};
		struct
		{
		private:
			uint8_t __pad0[0x70];
		public:
			union
			{
				struct
				{
					uint8_t toie : 1;
					uint8_t ociea : 1;
					uint8_t ocieb : 1;
				};
				uint8_t timsk;
			};
		};
		struct
		{
		private:
			uint8_t __pad1[0xB0];
		public:
			union
			{
				struct
				{
					uint8_t wgm0 : 2;
					uint8_t      : 2;
					uint8_t comb : 2;
					uint8_t coma : 2;
					uint8_t cs   : 3;
					uint8_t wgm1 : 1;
					uint8_t      : 2;
					uint8_t focb : 1;
					uint8_t foca : 1;
				};
				struct
				{
					uint8_t tccra;
					uint8_t tccrb;
				};
				uint16_t tccr;
			};
		};
		struct
		{
		private:
			uint8_t __pad2[0xB2];
		public:
			uint8_t tcnt;
			union
			{
				uint8_t tcnt;
				struct
				{
					uint8_t tcntl;
				};
			};
		};
		struct
		{
		private:
			uint8_t __pad3[0xb3];
		public:
			union
			{
				uint8_t ocra;
				struct
				{
					uint8_t ocral;
				};
			};
			union
			{
				uint8_t ocrb;
				struct
				{
					uint8_t ocrbl;
				};
			};
		};
		struct
		{
		private:
			uint8_t __pad4[0xB6];
		public:
			union
			{
				struct
				{
					uint8_t tcrub     : 2;
					uint8_t ocrbub    : 1;
					uint8_t ocraub    : 1;
					uint8_t tcnub     : 1;
					uint8_t as        : 1;
					uint8_t extclk    : 1;
					uint8_t extclkamr : 1;
				};
				uint8_t assr;
			};
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
		wgm0=form&0x3;
		wgm1=(form>>2)&0x1;
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
		return (WaveForms)(wgm0|(wgm1<<2));
	}
}__attribute__((packed));
}
}
}
}
