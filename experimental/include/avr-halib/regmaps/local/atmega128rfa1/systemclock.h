#pragma once

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{

/** \brief Regmap for System Clock **/
class Systemclock : public base::LocalRegMap,
{
public:
	enum Parameters
	{
		numPS=8
	};
	
	/** \brief System Clock prescalers**/
	enum Prescalers
	{
		ps2=0,			/**< \f$ f_{cpu}=f_{RC} \f$ **/
		ps4,				/**< \f$ f_{cpu}=\frac{f_{RC}}{4} \f$ **/
		ps8,				/**< \f$ f_{cpu}=\frac{f_{RC}}{8} \f$ **/
		ps16,				/**< \f$ f_{cpu}=\frac{f_{RC}}{16} \f$ **/
		ps32,				/**< \f$ f_{cpu}=\frac{f_{RC}}{32} \f$ **/
		ps64,				/**< \f$ f_{cpu}=\frac{f_{RC}}{64} \f$ **/
		ps128,				/**< \f$ f_{cpu}=\frac{f_{RC}}{128} \f$ **/
		ps1=0xf				/**< \f$ f_{cpu}=f_{RC} \f$ **/
	};
	
	
	union
	{
		struct
		{
			uint8_t __base[0x61];
			union
			{
				struct
				{
					uint8_t clkps : 4;
					uint8_t : 3;
					bool clkpce : 1;
				};
				uint8_t clkpr;
			};
		};
	};
	
	/** \brief setup prescaler of System Clock
	 * sets the System Clock prescaler as desired 
	 * 
	 * this method needs compile time optimization (timing is critical)
	 * \param ps Prescaler to setup
	 **/
	void setps(Prescalers ps)
	{
		clkpr=0x00; //ignore previous content
		clkpce=true;
		sync(); //from base::LocalRegMap
		clkpr=0x00;
		clkps=ps;
		sync();
	}
}__attribute__((packed));
}
}
}
}
