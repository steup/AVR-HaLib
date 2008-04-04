
struct ADConv
{
	volatile uint8_t : 0x78 * 8;
	volatile uint8_t adcl;
	volatile uint8_t adch;
	volatile uint8_t adcsra;
	volatile uint8_t adcsrb;
	volatile uint8_t admux;
	volatile uint8_t :8;
	volatile uint8_t didr0;
	
	enum {ps2 = 1, ps4 = 2, ps8 = 3, ps16 = 4, ps32 = 5, ps64 = 6, ps128 = 7 };
	
	
	// a way to encapsulate interrupt symbol to use in device specific structure
// 	// mainly for internal use, syntax not nice at all 
// 	template<class T, void (T::*Fxn)()>
// 	static void setRecvInterrupt(T & obj)
// 	{
// 		redirectISRMF(SIG_ADC, Fxn, obj);
// 	}
// 	
};

#include<avr/io.h>

/**
 *	\brief	
 *
 *	\warning	TODO: Problem mit Nebenläufigkeit beschreiben, Lösung: entweder bei interrrupt oder im normalen programmablaufs
 *
 */
template <class Return_Type, class ADC_Regmap, class Controller_Configuration>
	class AnalogDigitalConverter
{
#define rm (*(ADC_Regmap*)0x0)
        /// Zeiger, der auf Wert-Puffer des gerade abgefragten Sensors zeigt. Null, wenn im Moment keine AD-Wandlung im Gange ist.
	volatile Return_Type * target;
	/// Zeiger, der auf Variable des gerade abgefragten Sensors zeigt, die angibt, ob bereits ein neuer Sensorwert vorliegt. Null, wenn keine Wandlung im Gange.
	volatile bool * done;
public:	
	enum
	{
		prescaler = Controller_Configuration::controller_clk/200000UL,
		recommendedPrescalar = prescaler > 64? (ADC_Regmap::ps128) : prescaler > 32? (ADC_Regmap::ps64) :prescaler > (ADC_Regmap::ps32)? 5:prescaler > 8? (ADC_Regmap::ps16):prescaler > 4? (ADC_Regmap::ps8):prescaler > 2? (ADC_Regmap::ps4):(ADC_Regmap::ps2)
	};
	
	AnalogDigitalConverter():target(0),done(0)
	{}
	
	
	bool getValue(Return_Type &target, uint8_t mux, uint8_t prescaler = recommendedPrescalar)
	{
		if (rm.adcsra & (1<<ADSC))
			return false;
		
		if (this->target != 0)
			return false;	// AD-Wandlung im Gange
		rm.admux = mux|(1<<ADLAR); //| (0 << REFS1) | (1 << REFS0);			// AD-Kanal, Ausgabe linksbndig,(1<<ADLAR) Vergleichsspannung
		rm.adcsra = (1<<ADEN)|(1 << ADSC)|(prescaler << ADPS0);	// Starte AD-Wandlung ADSC, Interupt enable|(1 << ADIE)
		while (rm.adcsra & (1<<ADSC));
		
		target = rm.adcl;
		target |=(rm.adch<<8);
		return true;
	}
};
#if 0
template <class ADC_Regmap, class Controller_Configuration>
class AnalogDigitalConverter<uint8_t, ADC_Regmap, Controller_Configuration>;

template <class ADC_Regmap,class Controller_Configuration>
bool AnalogDigitalConverter<uint8_t, ADC_Regmap, Controller_Configuration>::getValue(uint8_t &target, uint8_t mux, uint8_t prescaler)
{
	if (rm.adcsra & (1<<ADSC))
		return false;
	
	if (this.target != 0)
		return false;	// AD-Wandlung im Gange
	rm.admux = mux|(1<<ADLAR); //| (0 << REFS1) | (1 << REFS0);			// AD-Kanal, Ausgabe linksbndig,(1<<ADLAR) Vergleichsspannung
	rm.adcsra = (1<<ADEN)|(1 << ADSC)|(prescaler << ADPS0);	// Starte AD-Wandlung ADSC, Interupt enable|(1 << ADIE)
	while (rm.adcsra & (1<<ADSC));
	
	target = rm.adch;
	return true;
}

template <class Return_Type, class ADC_Regmap, class Controller_Configuration>
bool AnalogDigitalConverter<Return_Type, ADC_Regmap, Controller_Configuration>::getValue(Return_Type &target, uint8_t mux, uint8_t prescaler)
{
	if (rm.adcsra & (1<<ADSC))
		return false;
	
	if (this.target != 0)
		return false;	// AD-Wandlung im Gange
	rm.admux = mux|(1<<ADLAR); //| (0 << REFS1) | (1 << REFS0);			// AD-Kanal, Ausgabe linksbndig,(1<<ADLAR) Vergleichsspannung
	rm.adcsra = (1<<ADEN)|(1 << ADSC)|(prescaler << ADPS0);	// Starte AD-Wandlung ADSC, Interupt enable|(1 << ADIE)
	while (rm.adcsra & (1<<ADSC));
	
	target = rm.adcl;
	target |=(rm.adch<<8);
	return true;
}
#endif
#undef rm