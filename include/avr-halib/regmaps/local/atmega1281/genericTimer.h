#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <avr-halib/regmaps/local/atmega128rfa1/timerCommon.h>
#include <stdint.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{
namespace helpers
{
	template<typename desc>
	struct GenericTimer : public base::LocalRegMap, public CommonTimerDefinitions
	{
		struct Parameters
		{
			static const bool asyncCapability=false;
			static const uint8_t numOCU = 3;
			static const uint8_t numPS = 5;
		};

		typedef uint16_t ValueType;
        static const ValueType maxValue = 0xFFFF;


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

		enum WaveForms
		{
			normal=0,
			phaseCorrectPWM8,
			phaseCorrectPWM9,
			phaseCorrectPWM10,
			ctc,
			fastPWM8,
			fastPwM9,
			fastPWM10,
			phaseFreqCorrectPWMICR,
			phaseFreqCorrectPWMOCM,
			phaseCorrectPWMICR,
			phaseCorrectPWMOCM,
			ctcICR,
			FastPWMICR=14,
			FastPWMOCM=15
		};

		typedef typename desc::InterruptMap InterruptMap;
			
		union
		{
			struct
			{
				uint8_t __pad0[desc::ocmOutput];
				struct
				{
					uint8_t : desc::ocmOffset;
					uint8_t ocmAOutput : 1;
					uint8_t ocmBOutput : 1;
					uint8_t ocmCOutput : 1;
				};
			};
			struct
			{
				uint8_t __pad1[desc::tifr];
				union
				{
					struct
					{
						uint8_t tovf : 1;
						uint8_t ocaf : 1;
						uint8_t ocbf : 1;
						uint8_t occf : 1;
						uint8_t      : 1;
						uint8_t icf  : 1;
					};
					uint8_t tifr;
				};
			};
			struct
			{
				uint8_t __pad2[desc::timsk];
				union
				{
					struct
					{
						uint8_t toie  : 1;
						uint8_t ociea : 1;
						uint8_t ocieb : 1;
						uint8_t ociec : 1;
						uint8_t       : 1;
						uint8_t icie  : 1;
					};
					uint8_t timsk;
				};
			};
			struct
			{
				uint8_t __pad3[desc::tccr];
				union
				{
					struct
					{
						uint8_t wgm0 : 2;
						uint8_t comc : 2;
						uint8_t comb : 2;
						uint8_t coma : 2;
						uint8_t cs   : 3;
						uint8_t wgm1 : 2;
						uint8_t      : 1;
						uint8_t ices : 1;
						uint8_t icnc : 1;
						uint8_t      : 5;
						uint8_t focc : 1;
						uint8_t focb : 1;
						uint8_t foca : 1;
					};
					struct
					{
						uint8_t tccra;
						uint8_t tccrb;
						uint8_t tccrc;
					};
				};
			};
			struct
			{
				uint8_t __pad4[desc::tcnt];
				union
				{
					uint16_t tcnt;
					struct
					{
						uint8_t tcntl;
						uint8_t tcnth;
					};
				};
			};
			struct
			{
				uint8_t __pad5[desc::icr];
				union
				{
					uint16_t icr;
					struct
					{
						uint8_t icrl;
						uint8_t icrh;
					};
				};
			};
			struct
			{
				uint8_t __pad6[desc::ocr];
				union
				{
					uint16_t ocra;
					struct
					{
						uint8_t ocral;
						uint8_t ocrah;
					};
				};
				union
				{
					uint16_t ocrb;
					struct
					{
						uint8_t ocrbl;
						uint8_t ocrbh;
					};
				};
				union
				{
					uint16_t ocrc;
					struct
					{
						uint8_t ocrcl;
						uint8_t ocrch;
					};
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
		wgm1=(form>>2)&0x3;
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
}
