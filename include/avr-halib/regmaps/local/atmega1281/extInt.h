#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <avr-halib/interrupts/atmega128rfa1/extInt.h>
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
		template<typename config>
		struct ExtIntGenerator : public config, public base::LocalRegMap
		{

			typedef typename config::Sensitivity Sensitivity;

			enum Types
			{
				external =0,
				software
			};


			typedef interrupts::atmega128rfa1::ExternalInterruptTemplate<config::nr> InterruptMap;

			union{
				struct{
					uint8_t _pad0[config::portAddress];
					struct{
						uint8_t         		: config::portOffset + config::nr;
						Types 	type    		: 1;
					};
					struct{
						uint8_t         		: config::portOffset+ config::nr;
						uint8_t trigger 		: 1;
					};
				};
				struct{
					uint8_t _pad1[config::flagAddress];
					struct{
						uint8_t	         		: config::nr;
						uint8_t flag    		: 1;
					};
					struct{
						uint8_t 	    		: config::nr;
						uint8_t mask    		: 1;
					};
				};

				struct{
					uint8_t _pad2[config::controlAddress];
					struct
					{
						uint16_t 				: config::nr * 2;
						Sensitivity sensitivity	: 2;
					};
				};
										  
			};

		} __attribute__((packed));

		struct GlobalExtIntConfig
		{
			static const uint8_t baseInterrupt  = 1;
			static const uint8_t flagAddress    = 0x3C;
			static const uint8_t controlAddress = 0x69;
		};

		struct LowerExtIntConfig : public GlobalExtIntConfig
		{
			enum Sensitivity
			{
				lowLevel =0,
				falling  =2,
				rising   =3
			};

			static const int8_t  portOffset  = 0;
			static const uint8_t portAddress = 0x2A;
		};

		struct HigherExtIntConfig : public GlobalExtIntConfig
		{
			enum Sensitivity
			{
				lowLevel =0,
				both	 =1,
				falling  =2,
				rising   =3
			};

			static const int8_t  portOffset  = 0;
			static const uint8_t portAddress = 0x2D;
		};

		struct ExtInt0 : public LowerExtIntConfig
		{
			static const uint8_t nr = 0;
		};

		struct ExtInt1 : public LowerExtIntConfig
		{
			static const uint8_t nr = 1;
		};

		struct ExtInt2 : public LowerExtIntConfig
		{
			static const uint8_t nr = 2;
		};		

		struct ExtInt3 : public LowerExtIntConfig
		{
			static const uint8_t nr = 3;
		};

		struct ExtInt4 : public HigherExtIntConfig
		{
			static const uint8_t nr = 4;
		};

		struct ExtInt5 : public HigherExtIntConfig
		{
			static const uint8_t nr = 5;
		};

		struct ExtInt6 : public HigherExtIntConfig
		{
			static const uint8_t nr = 6;
		};

		struct ExtInt7 : public HigherExtIntConfig
		{
			static const uint8_t nr = 7;
		};
	}

	typedef helpers::ExtIntGenerator< helpers::ExtInt0 > ExternalInterrupt0;
	typedef helpers::ExtIntGenerator< helpers::ExtInt1 > ExternalInterrupt1;
	typedef helpers::ExtIntGenerator< helpers::ExtInt2 > ExternalInterrupt2;
	typedef helpers::ExtIntGenerator< helpers::ExtInt3 > ExternalInterrupt3;
	typedef helpers::ExtIntGenerator< helpers::ExtInt4 > ExternalInterrupt4;
	typedef helpers::ExtIntGenerator< helpers::ExtInt5 > ExternalInterrupt5;
	typedef helpers::ExtIntGenerator< helpers::ExtInt6 > ExternalInterrupt6;
	typedef helpers::ExtIntGenerator< helpers::ExtInt7 > ExternalInterrupt7;
}
}
}
}
