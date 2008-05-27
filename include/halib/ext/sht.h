#include <stdint.h>
#include "halib/share/delay.h"
#include "halib/avr/portmap.h"


template<class Portmap>
	class Sensibus
{
	private:
		
		enum{wait=0};
		
		void toggelclk()
		{	
			UsePortmap(pm, Portmap);
			
 			Sync(pm); 
			
			pm.sck.ddr = Portmap::out;
			pm.sck.port=1;
 			
 			Sync(pm); 
			
 			delay_us(wait);
			
			pm.sck.port =0;
			Sync(pm);
			
 			delay_us(wait);
		}
	public:
		enum commands
		{
			measureTemperature=0x03,
			measureHumidity=0x05,
			readStatusRegister=0x07,
			writeStatusRegister=0x06,
			softReset=0x1e
		};
		
		void transnmissionStart()
		{
			UsePortmap(pm, Portmap);
			
			pm.data.ddr = Portmap::out;
			pm.sck.ddr = Portmap::out;
			
			Sync(pm); 

			pm.data.port = 1; 
			
			Sync(pm);
			
			pm.sck.port = 1;
			
			Sync(pm); 
// 			delay_us(wait);
			
			pm.data.port = 0;
			
			Sync(pm); 
// 			delay_us(wait);
			
			pm.sck.port = 0;
			
			Sync(pm); 
			
			delay_us(wait);
			
			pm.sck.port = 1;
			
			Sync(pm); 
// 			delay_us(wait);
			pm.data.port = 1;
			
			Sync(pm); 
			delay_us(wait);
			pm.sck.port = 0;
			
			
			Sync(pm); 
// 			delay_us(wait);
			
		}
		
		bool writebyte(uint8_t byte)
		{
			UsePortmap(pm, Portmap);
			
			pm.data.ddr = Portmap::out;
			pm.sck.ddr = Portmap::out;
			Sync(pm); 
			
			for (uint8_t i = 0x80; i ; i >>= 1)
			{
				pm.data.port = byte & i;
				
				toggelclk();
			}
			
			pm.data.ddr = Portmap::in;			
			pm.data.port = true;
			Sync(pm); 
			
			pm.sck.port = 1;
			Sync(pm); 
			delay_us(2*wait);
			bool ret = !pm.data.pin;
			pm.sck.port = 0;
			pm.data.ddr= Portmap::out;
			Sync(pm);
			return ret;
			
		}
		
		bool readbyte(uint8_t &byte, bool ack)
		{
			UsePortmap(pm, Portmap);
			
			pm.sck.ddr = Portmap::out;
			pm.data.ddr= Portmap::in;
			Sync(pm);
			pm.data.port = true;
			Sync(pm);
			byte = 0;
			for (uint8_t i = 0x80; i ; i >>= 1)
			{
				Sync(pm);
				byte |= pm.data.pin ? i:0 ;
				toggelclk();
			}
			pm.data.ddr= Portmap::out;
			pm.data.port = !ack;
			toggelclk();
			pm.data.port = true;
			Sync(pm);
			return true;
		}
		
		void resetconnection()
		{
			UsePortmap(pm, Portmap);
			
			pm.data.ddr= Portmap::out;
			pm.sck.ddr = Portmap::out;
			Sync(pm);
			for (uint8_t i = 9; i ; i --)
			{
				pm.data.port= true;
				toggelclk();
			}
			transnmissionStart();
		}
		
		bool measurementIsFinished(){UsePortmap(pm, Portmap); Sync(pm); return !pm.data.pin;}
};

template<class Portmap>
	class SHTTemperatur: 
		public Sensibus<Portmap>
{
protected:
	typedef uint16_t ReturnType;
	union {ReturnType value; struct { uint8_t lo8; uint8_t hi8;};};
public:
	SHTTemperatur(){}
	
	/// returns true if process started by startGetValue() had been completed.
	bool isReady(){return true;}
	
	/// returns true if value-getting-process could be started
	bool startGetValue()
	{
		UsePortmap(pm, Portmap);
		
		Sensibus<Portmap>::resetconnection();
		
		if (!Sensibus<Portmap>::writebyte(Sensibus<Portmap>::measureTemperature))
			return false;
		
		pm.data.ddr= Portmap::in;
		pm.data.port = true;
		Sync(pm);
		
		while(!Sensibus<Portmap>::measurementIsFinished());
		if (!(Sensibus<Portmap>::readbyte(hi8, true) && Sensibus<Portmap>::readbyte(lo8, false)))
			return false;
		
		return true;
	}
	
	/// returns true if isReady() and writes gotten realworld-value to value
	bool getCachedValue(ReturnType &value)
	{
		if(isReady())
		{
			value = this->value;
			return true;
		}
		else	
			return false;
	}
};


template<class Portmap>
	class SHTHumidity: 
		public Sensibus<Portmap>
{
protected:
	typedef uint16_t ReturnType;
	union {ReturnType value; struct { uint8_t lo8; uint8_t hi8;};};
public:
	SHTHumidity(){}
	
	/// returns true if process started by startGetValue() had been completed.
	bool isReady(){return true;}
	
	/// returns true if value-getting-process could be started
	bool startGetValue()
	{
		UsePortmap(pm, Portmap);
		
		Sensibus<Portmap>::resetconnection();
		
		if (!Sensibus<Portmap>::writebyte(Sensibus<Portmap>::measureHumidity))
			return false;
		
		pm.data.ddr= Portmap::in;
		pm.data.port = true;
		Sync(pm);
		
		while(!Sensibus<Portmap>::measurementIsFinished());
		if (!(Sensibus<Portmap>::readbyte(hi8, true) && Sensibus<Portmap>::readbyte(lo8, false)))
			return false;
		
		return true;
	}
	
	/// returns true if isReady() and writes gotten realworld-value to value
	bool getCachedValue(ReturnType &value)
	{
		if(isReady())
		{
			value = this->value;
			return true;
		}
		else	
			return false;
	}
};



