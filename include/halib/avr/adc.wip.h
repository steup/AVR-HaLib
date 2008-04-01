struct ADC
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

template class AnalogDigitalConverter<class Return_Type, class ADC_Regmap>
{
#define rm (*(ADC_Regmap*)0x0)
        /// Zeiger, der auf Wert-Puffer des gerade abgefragten Sensors zeigt. Null, wenn im Moment keine AD-Wandlung im Gange ist.
	static volatile Return_Type * target;
	/// Zeiger, der auf Variable des gerade abgefragten Sensors zeigt, die angibt, ob bereits ein neuer Sensorwert vorliegt. Null, wenn keine Wandlung im Gange.
	static volatile bool * done;


public:
	void configure();
	bool getValue(Return_Type &target, uint8_t mux, uint8_t prescaler)
	{
		if (!rm.adcsra & (1<<ADEN))
			rm.adcsra = (1<<ADEN);
		else
			return false;
		
		if (this.target != 0)
			return false;	// AD-Wandlung im Gange
		//this.target = target;
		//done = fertig;
		//*done = false;
		rm.admux = mux; //| (0 << REFS1) | (1 << REFS0);			// AD-Kanal, Ausgabe linksbndig,(1<<ADLAR) Vergleichsspannung
		rm.adcsra = (1<<ADEN)|(1 << ADSC);	// Starte AD-Wandlung ADSC, Interupt enable|(1 << ADIE)
		return true;
	}
	uint8_t getRecommendedPrescalar()
	{
		return 200000/cpufrequency 
	}
	

}
#undef rm