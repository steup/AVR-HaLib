#include <stdint.h>
#include "halib/share/delay.h"
#include "halib/avr/portmap.h"

template<class Portmap>
	class sensibus
{
	private:
		
		enum{wait=0};
		
		void toggelclk()
		{	
			UsePortmapVolatile(pm, Portmap);
			
			Volatile(pm) 
			
			pm.sck.ddr = Portmap::out;
			pm.sck.port=1;
			Volatile(pm) 
			
 			delay_us(wait);
			
			pm.sck.port =0;
			Volatile(pm)
			
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
			UsePortmapVolatile(pm, Portmap);
			
			pm.data.ddr = Portmap::out;
			pm.sck.ddr = Portmap::out;
			
			pm.data.port = 1; 
			
			pm.sck.port = 1;
			
			Volatile(pm) 
			delay_us(wait);
			pm.data.port = 0;
			Volatile(pm) 
			delay_us(wait);
			pm.sck.port = 0;
			
			
			Volatile(pm) 
			delay_us(wait);
			
			pm.sck.port = 1;
			
			Volatile(pm) 
			delay_us(wait);
			pm.data.port = 1;
			
			Volatile(pm) 
			delay_us(wait);
			pm.sck.port = 0;
			
			
			Volatile(pm) 
			delay_us(wait);
			
		}
		
		bool writebyte(uint8_t byte)
		{
			UsePortmapVolatile(pm, Portmap);
			
			pm.data.ddr = Portmap::out;
			pm.sck.ddr = Portmap::out;
			Volatile(pm) 
			for (uint8_t i = 0x80; i ; i >>= 1)
			{
				pm.data.port = byte & i;
				Volatile(pm) 
				toggelclk();
			}
			
			pm.data.port = true;
			pm.data.ddr = Portmap::in;
			pm.sck.port = 1;
			Volatile(pm) 
			delay_us(2*wait);
			bool ret = !pm.data.pin;
			pm.sck.port = 0;
			pm.data.ddr= Portmap::out;
			return ret;
			
		}
		
		bool readbyte(uint8_t &byte, bool ack)
		{
			UsePortmapVolatile(pm, Portmap);
			
			pm.sck.ddr = Portmap::out;
			pm.data.ddr= Portmap::in;
			pm.data.port = true;
			Volatile(pm)
			byte = 0;
			for (uint8_t i = 0x80; i ; i >>= 1)
			{
				byte |= pm.data.pin ? i:0 ;
				toggelclk();
			}
			pm.data.ddr= Portmap::out;
			pm.data.port = !ack;
			toggelclk();
			pm.data.port = true;
			return true;
		}
		
		void resetconnection()
		{
			UsePortmapVolatile(pm, Portmap);
			
			pm.data.ddr= Portmap::out;
			pm.sck.ddr = Portmap::out;
			for (uint8_t i = 9; i ; i --)
			{
				pm.data.port= true;
				toggelclk();
			}
			transnmissionStart();
		}
};

template<class Portmap>
	class SHTTemperatur: 
		public sensibus<Portmap>
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
		UsePortmapVolatile(pm, Portmap);
		
		sensibus<Portmap>::resetconnection();
		
		if (!sensibus<Portmap>::writebyte(sensibus<Portmap>::measureTemperature))
			return false;
		
		pm.data.ddr= Portmap::in;
		pm.data.port = true;
		Volatile(pm)
		
		while(pm.data.pin);
		if (!(sensibus<Portmap>::readbyte(hi8, true) && sensibus<Portmap>::readbyte(lo8, false)))
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
		public sensibus<Portmap>
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
		UsePortmapVolatile(pm, Portmap);
		
		sensibus<Portmap>::resetconnection();
		
		if (!sensibus<Portmap>::writebyte(sensibus<Portmap>::measureHumidity))
			return false;
		
		pm.data.ddr= Portmap::in;
		pm.data.port = true;
		Volatile(pm)
		
		while(pm.data.pin);
		if (!(sensibus<Portmap>::readbyte(hi8, true) && sensibus<Portmap>::readbyte(lo8, false)))
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



