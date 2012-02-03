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
class Systemclock : public base::LocalRegMap
{
public:
	enum Parameters
	{
		numPS=8
	};
	
	/** \brief System Clock prescalers**/
	enum Prescalers
	{
		ps1=0,			/**< \f$ f_{cpu}=f_{source} \f$ **/
		ps2,				/**< \f$ f_{cpu}=\frac{f_{source}}{2} \f$ **/
		ps4,				/**< \f$ f_{cpu}=\frac{f_{source}}{4} \f$ **/
		ps8,				/**< \f$ f_{cpu}=\frac{f_{source}}{8} \f$ **/
		ps16,				/**< \f$ f_{cpu}=\frac{f_{source}}{16} \f$ **/
		ps32,				/**< \f$ f_{cpu}=\frac{f_{source}}{32} \f$ **/
		ps64,				/**< \f$ f_{cpu}=\frac{f_{source}}{64} \f$ **/
		psnone=0x0f,	/**< \f$ f_{cpu}=f_{RC} special value for 16MHz RC \f$ **/
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
	 * this method should not be interupted
	 * you may want to disable interrups
	 * timing is critical
	 * 
	 * \param ps Prescaler to setup
	 **/
//#include <avr-halib/share/interruptLock.h>
// 		avr_halib::locking::GlobalIntLock lock;
	void setps(Prescalers ps)
	{
		
		asm volatile(
			"sts     0x0061, %1""\n\t"
			"sts     0x0061, %0""\n\t"
			:
			:"r" ((uint8_t) ps),"r" ((uint8_t) 0x08)
		);
		/* nice Regmap version
		//this method needs compile time optimization (timing is critical)
		clkpr=0x00; //ignore previous content
		clkpce=true;
		sync(); //from base::LocalRegMap
		clkpr=0x00;
		clkps=ps;
		sync();
		*/
	}
}__attribute__((packed));
}
}
}
}
