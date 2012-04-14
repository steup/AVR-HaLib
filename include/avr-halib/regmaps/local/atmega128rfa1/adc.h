#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <avr-halib/interrupts/atmega128rfa1/adc.h>
#include <stdint.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{
/**
 *	\brief		Register map for ADC of atmega128rfa1
 *
 * 	for further detail of register content, see ATMEL atmega128rfa1 reference manual
 */

struct ADCRegMap : public base::LocalRegMap
{
public:
	typedef interrupts::atmega128rfa1::Adc InterruptMap;

private:
	uint8_t __base[0x78];
public:
	union
	{
		struct
		{
			uint8_t adcl:8;	
			uint8_t adch:8;
		};
		struct
		{
			uint16_t adc:16;
		};
	};
	
	uint8_t adps : 3;
	bool	adie : 1;
	bool	adif : 1;
	bool	adate: 1;
	bool	adsc : 1;
	bool	aden : 1;
	uint8_t adts : 3;
	bool	mux5  : 1;
	uint8_t  : 2;
	bool	acme : 1;
	uint8_t  : 1;
	union
	{
		uint8_t  admux	:8;
		struct{
			uint8_t  mux	:5;
			bool	 adlra	:1;
			uint8_t  refs	:2;
		};
	};
private:
	uint8_t __pad0 :8;
public:
	uint8_t didr0;

public:

    struct Prescalers
    {
        /**\brief Prescalers for adc operation**/
        enum PrescalerType
        {
            ps2=1,	/**< \f$ clk_{ADC} = \frac{1}{2}   \cdot clk \f$**/
            ps4,	/**< \f$ clk_{ADC} = \frac{1}{4}   \cdot clk \f$**/
            ps8,	/**< \f$ clk_{ADC} = \frac{1}{8}   \cdot clk \f$**/
            ps16,	/**< \f$ clk_{ADC} = \frac{1}{16}  \cdot clk \f$**/
            ps32,	/**< \f$ clk_{ADC} = \frac{1}{32}  \cdot clk \f$**/
            ps64,	/**< \f$ clk_{ADC} = \frac{1}{64}  \cdot clk \f$**/
            ps128	/**< \f$ clk_{ADC} = \frac{1}{128} \cdot clk \f$**/
        };
    };
    typedef Prescalers::PrescalerType PrescalerType;


    struct References
    {
        /**\brief Reference voltages**/
        enum ReferenceType
        {
            external,	/**< External reference connected to AREF**/
            vcc,		/**< Vcc voltage as reference**/
            internal=3	/**< Internal 2.56V bandgap reference**/
        };
    };
    typedef References::ReferenceType ReferenceType;

    struct Channels
    {
        /**\brief Mux channels**/
        enum ChannelType
        {
            channel0=0,
            channel1=1,
            channel2=2,
            channel3=3,
            channel4=4,
            channel5=5,
            channel6=6,
            channel7=7,
            bandgap11=0x1e
        };
    };
    typedef Channels::ChannelType ChannelType;
}__attribute__((packed));
}
}
}
}
