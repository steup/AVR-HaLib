#pragma once

#include <avr-halib/config/timer.h>
#include <avr-halib/interrupts/at90can128/timer2.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace at90can128
{

	/** \brief Regmap for asynchronous timer 2 **/
	struct Timer2 : public base::LocalRegMap
	{
		template< PrescalerType ps>
		struct PrescalerToClockSelect
		{
			static const uint8_t cs = (ps == Prescalers::noClock) ? 0x0:
										(ps == Prescalers::ps1    ) ? 0x1:
										  (ps == Prescalers::ps8    ) ? 0x2:
										    (ps == Prescalers::ps32    ) ? 0x3:
											  (ps == Prescalers::ps64   ) ? 0x4:
											    (ps == Prescaler::ps128   ) ? 0x5:
											      (ps == Prescalers::ps256  ) ? 0x6: 0x7;
		};

		template< ExternalClockEdgeType edgeType >
		struct ExternalClockEdgeToClockSelect
		{
			static const uint8_t cs = 0x0;
		};

		template< OutputModeType om, WaveFormType wf >
		struct OutputModeTypeToOCM
		{
			static const uint8_t ocm = (om == OutputModes::noOutput ) ? 0x0:
										 (om == OutputModes::toggle   ) ? 0x1:
										   (om == OutputModes::clear    ) ? 0x2: 0x3;
		};

		struct NormalToWGM
		{
			static const uint8_t wgm0 = 0x0;
			static const uint8_t wgm1 = 0x0;
		};

		template< MatchInputType m >
		struct ClearMatchToWGM
		{
			static const uint8_t wgm0 = 0x0;
			static const uint8_t wgm1 = 0x1;
		};


		template< PWMCorrectionType c, PWMAccuracyType a, MatchInputType m >
		struct PWMConfigToWGM
		{
			static const uint8_t wgm0 = 0x1;
			static const uint8_t wgm1 = (c == PWMCorrections::phaseCorrect) ? 0x0 : 0x1;
		};

		struct PrescalerIterator
		{
			static const PrescalerType ps = ps1;
			static bool hasNext = true;
			struct next
			{
				static const PrescalerType ps = ps8;
				static bool hasNext = true;
				struct next
				{
					static const PrescalerType ps = ps32;
					static bool hasNext = true;
					struct next
					{
						static const PrescalerType ps = ps64;
						static bool hasNext = true;
						struct next
						{
							static const PrescalerType ps = ps128;
							static bool hasNext = true;
							struct next
							{
								static const PrescalerType ps = ps256;
								static bool hasNext = true;
								struct next
								{
									static const PrescalerType ps = ps1024;
									static bool hasNext = false;
								};
							};
						};
					};
				};
			};
		};

		enum Parameters
		{
			rtc      = true,
			numOCUs  = 1
		};

		typedef interrupts::at90can128::Timer2IntMap InterruptMap;

		typedef uint8_t ValueType;
		union
		{
			struct
			{
				uint8_t __base[0x24];
				uint8_t     : 4;
				uint8_t ocm : numOCUs;
			};

			struct{
				uint8_t __pad0[0x37];
				union
				{
					struct
					{
						uint8_t tov : 1;
						uint8_t ocf : numOCUs;
					};
					uint8_t tifr;
				};
			};
			struct
			{
				uint8_t __pad1[0x70];
				union
				{
					struct
					{
						uint8_t toie : 1;
						uint8_t ocie : numOCUs;
					};
					uint8_t timsk;
				};
			};
			struct
			{
				uint8_t __pad2[0xB0];
				union
				{
					struct
					{
						uint8_t cs   : 3;
						uint8_t wgm1 : 1;
						uint8_t coma : 2;
						uint8_t wgm0 : 1;
						uint8_t foc  : numOCUs;
					};
					uint8_t tccr;
				};
			};
			struct{
				uint8_t __pad3[0xB2];
				uint8_t tcnt;
				uint8_t ocr[numOCUs];
			};
			struct
			{
				uint8_t __pad4[0xB6];
				union
				{
					struct
					{
						uint8_t tcrub : 1;
						uint8_t ocrub : 1;
						uint8_t tcnub : 1;
						uint8_t as    : 1;
						uint8_t extclk : 1;
					};
					uint8_t assr;
				};
			};
		};
}__attribute__((packed));
}
}
}
}
