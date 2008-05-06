#include <stdint.h>
#include "halib/share/delay.h"

template<class Portmap>
	class sensibus
{
	private:
		void toggelclk()
		{	
			pm.sck=1
			delay_us(5);
			pm.sck=0
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
			pm.ddr.data = pm::out;
			pm.ddr.sck = pm::out;
			pm.data = 1; 
			
			pm.sck = 1;
			delay_us(5);
			pm.data = 0;
			delay_us(5);
			pm.sck = 0;
			
			delay_us(5);
			
			pm.sck = 1;
			delay_us(5);
			pm.data = 1;
			delay_us(5);
			pm.sck = 0;
			
			delay_us(5);
			
		}
		
		bool writebyte(uint8_t byte)
		{
			pm.ddr.data = pm::out;
			pm.ddr.sck = pm::out;
			for (uint8_t i = 0x80; i ; i >> 1)
			{
				pm.data= byte && i;
				toggelclk();
			}
			pm.data = true;
			pm.ddr.data = pm::in;
			pm.sck = 1;
			delay_us(5);
			bool ret = pm.pin.data;
			pm.sck = 0;
			pm.ddr.data = pm::out;
			return ret;
			
		}
		
		bool readbyte(uint8_t &byte, bool ack)
		{
			pm.ddr.sck = pm::out;
			pm.ddr.data = pm::in;
			pm.data = true;
			byte = 0;
			for (uint8_t i = 0x80; i ; i >> 1)
			{
				byte |= pm.pin.data ? i:0 ;
				toggelclk();
			}
			pm.ddr.data = pm::out;
			pm.data = !ack;
			toggelclk();
			pm.data = true;
			return true;
		}
		
		void resetconnection()
		{
			pm.ddr.data = pm::out;
			pm.ddr.sck = pm::out;
			for (uint8_t i = 9; i ; i --)
			{
				pm.data= true;
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
	ReturnType value;
public:
	SHTTemperatur(){}
	
	/// returns true if process started by startGetValue() had been completed.
	bool isReady(){return true;}
	
	/// returns true if value-getting-process could be started
	bool startGetValue()
	{
		UseRegmapVolatile(rmv, );
		resetconnection();
		transnmissionStart();
		
		struct{ uint8_t low8; uint8_t hi8} & val = value;
		
		if (!writebyte(commands::measureTemperature))
			return false;
		while(rmv.data);
		if (!(readbyte(val.hi8,true)&&readbyte(val.low8,false)))
			return false;
		
		return true;
	}
	
	/// returns true if isReady() and writes gotten realworld-value to value
	bool getCachedValue(ReturnType &value)
	{
		if(isReady())
		{
			value = this.value;
			return true;
		}
		else	
			return false;
	}
};

};

template<class portmap>
	class SHTHumidity: 
		public sensibus<portmap>
{


};