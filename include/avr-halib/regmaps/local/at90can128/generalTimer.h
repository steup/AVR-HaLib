#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <avr-halib/config/timer.h>
#include <stdint.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace at90can128
{
	namespace generalTimer
	{
		using namespace config::timer;

		template<typename desc>
		struct 16Bit : public base::LocalRegMap
		{
			template< PrescalerType ps>
			struct PrescalerToClockSelect
			{
				static const uint8_t cs = (ps == Prescalers::noClock) ? 0x0:
											(ps == Prescalers::ps1    ) ? 0x1:
											  (ps == Prescalers::ps8    ) ? 0x2:
												(ps == Prescalers::ps64   ) ? 0x3:
												  (ps == Prescalers::ps256  ) ? 0x4: 0x5;
			};

			template< ExternalClockEdgeType edgeType >
			struct ExternalClockEdgeToClockSelect
			{
				static const uint8_t cs = (edgeType == ExternalClockEdges::falling) ? 0x6 : 0x7;
			};

			template< OutputModeType om, WaveFormType wf  >
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
				static const uint8_t wgm1 = (m == MatchInputs::OCRA) ? 0x1 : 0x3;
			};


			template< PWMCorrectionType c, PWMAccuracyType a, MatchInputType m >
			struct PWMConfigToWGM
			{
				static const uint8_t wgm0 = (a == PWMAccuracys::8Bit               ) ? 0x1:
											  (a== PWMAccuracys::9Bit                ) ? 0x2:
												(a == PWMAccuracys::10Bit              ) ? 0x3:
												  (c == PWMCorrections::phaseAndFrequency) ?
													( (m == MatchInputs::OCRA) ? 0x0 : 0x1 ):
													( (m == MatchInputs::OCRA) ? 0x2 : 0x3 );
				static const uint8_t wgm1 = (c == PWMCorrections::phaseCorrect) ?
											  ( (a == PWMAccuracys::dynamic) ? 0x2 : 0x0 ):
											  (c == PWMCorrections::none) ?
												( (a == PWMAccuracys::dynamic) ? 0x3 : 0x1 ):
												  ( m == MatchInputs::OCRA) ? : 0x1 : 0x0;
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
						static const PrescalerType ps = ps64;
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

			enum Parameters
			{
				rtc = false,
				numOCUs = 3
			};

			typedef uint16_t ValueType;
			typedef typename desc::InterruptMap InterruptMap;
				
			union
			{
				struct
				{
					uint8_t __pad0[desc::ocmOutput];
					struct
					{
						uint8_t     : desc::ocmOffset;
						uint8_t ocm : numOCUs;
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
							uint8_t ocf  : numOCUs;
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
							uint8_t toie : 1;
							uint8_t ocie : numOCUs;
							uint8_t      : 1;
							uint8_t icie : 1;
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
							uint8_t foc  : numOCUs;
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
					uint16_t ocr[numOCUs];
				};
			};
		}__attribute__((packed));
	}
}
}
}
}
