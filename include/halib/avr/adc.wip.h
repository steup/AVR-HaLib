
struct ADConv
{
private:
	const uint8_t __base[0x78];// 	uint8_t :0x78*8;
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
	uint8_t  : 3;
	bool	acme : 1;
	uint8_t  : 1;
	uint8_t  mux	:5;
	bool	 adlra	:1;
	uint8_t  refs	:2;
private:	
	volatile uint8_t __pad0	:8;
public:
	volatile uint8_t didr0;
	
	enum {ps2 = 1, ps4 = 2, ps8 = 3, ps16 = 4, ps32 = 5, ps64 = 6, ps128 = 7 };
	enum {ref_aref = 0, ref_avcc = 1, ref_internal2_56 = 3};
	
	
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
#include<halib/avr/regmaps.h>

/**
 *	\brief	
 *
 *	\warning	TODO: Problem mit Nebenläufigkeit beschreiben, Lösung: entweder bei interrrupt oder im normalen programmablaufs
 *
 */
 template <class Return_Type,class ADC_Regmap, class Controller_Configuration>
	class AnalogDigitalConverter
{
        /// Zeiger, der auf Wert-Puffer des gerade abgefragten Sensors zeigt. Null, wenn im Moment keine AD-Wandlung im Gange ist.
// 	volatile Return_Type * target;
	/// Zeiger, der auf Variable des gerade abgefragten Sensors zeigt, die angibt, ob bereits ein neuer Sensorwert vorliegt. Null, wenn keine Wandlung im Gange.
// 	volatile bool * done;
	
	template<class Type,class ADC_Regmap_N>
	class helper
	{
		public:
		static void set_adlra();
		static void write_target(Type &target);
	};
	
	template<class ADC_Regmap_N>
	class helper<uint8_t,ADC_Regmap_N>
	{	
		public:
		static void set_adlra(){UseRegmap(rm, ADC_Regmap_N);rm.adlra = 1;}
		static void write_target(uint8_t &target){UseRegmap(rm, ADC_Regmap_N);target = rm.adch;}
	};
	
	template<class ADC_Regmap_N>
	class helper<uint16_t,ADC_Regmap_N>
	{
		public:
		static void set_adlra(){UseRegmap(rm, ADC_Regmap_N);rm.adlra = 0;}
		static void write_target(uint16_t &target){UseRegmap(rm, ADC_Regmap_N);target = rm.adc;}
	};
		

public:	
	enum
	{
		prescaler = Controller_Configuration::controller_clk/200000UL,
		recommendedPrescalar = prescaler > 64? (ADC_Regmap::ps128) : prescaler > 32? (ADC_Regmap::ps64) :prescaler > (ADC_Regmap::ps32)? 5:prescaler > 8? (ADC_Regmap::ps16):prescaler > 4? (ADC_Regmap::ps8):prescaler > 2? (ADC_Regmap::ps4):(ADC_Regmap::ps2)
	};
	
	AnalogDigitalConverter() //: rm((*(ADC_Regmap*)0x0))//:target(0),done(0)
	{}
	
	
	
	bool getValue(Return_Type &target, uint8_t mux,uint8_t reference ,uint8_t prescaler = recommendedPrescalar)
	{
		UseRegmap(rm, ADC_Regmap);
		UseRegmapVolatile(rmv, ADC_Regmap);
		
		if (rm.adsc)
			return false;
		
	/*	if (this->target != 0)
			return false;	// AD-Wandlung im Gange
	*/	rm.refs = reference;
		
		helper<Return_Type,ADC_Regmap>::set_adlra();	// AD-Kanal, Ausgabe 
		
		rm.mux = mux;			// AD-Kanal,  Vergleichsspannung
		rm.aden = true;
		rm.adps = prescaler;
		rm.adsc = true;			// Starte AD-Wandlung ADSC, Interupt enable|(1 << ADIE)
		while (rmv.adsc);
		
		
		helper<Return_Type,ADC_Regmap>::write_target(target);
		
		return true;
	}
};


