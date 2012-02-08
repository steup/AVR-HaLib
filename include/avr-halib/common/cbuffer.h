#pragma once
#include "avr-halib/share/queuebuffer.h"
#include "avr-halib/share/delegate.h"

template <class BaseCDevice, class length_t = uint8_t, length_t oBufLen = 255>
		class COutBuffer: public BaseCDevice
{
	private:	
	//protected:
		QueueBuffer<char, length_t, oBufLen> outBuffer;
		void sendonReady()
		{
			while(!outBuffer.isFull())
				if(onReady.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onReady();
		}
	public:
		Delegate<> onReady;
		void enableonReady(){
			
			BaseCDevice::activateonReady();
			sendonReady();
		}
		
		/// forwards char on BaseCDevice ready
		void putonReady(){
			char c;
			if( outBuffer.get(c) )
				BaseCDevice::put(c);
			else
				BaseCDevice::disableonReady();
			sendonReady();
		}
				
		/// Writes a character into the output buffer
		void put(const char c)
		{
			typedef COutBuffer<BaseCDevice, length_t, oBufLen> thisclass;
			
			outBuffer.put(c);
			BaseCDevice::onReady.template bind< thisclass , &thisclass::putonReady >(this);//für Signal anmelden
			BaseCDevice::enableonReady();
		
		}
		
		bool ready()__attribute__ ((always_inline))
		{
			return !outBuffer.isFull();
		}
	
};	

template <class BaseCDevice, class length_t = uint8_t, length_t iBufLen = 255>
		class CInBuffer: public BaseCDevice
{
	private:	//protected:
		QueueBuffer<char, length_t, iBufLen> inBuffer;
		void sendonReceive()
		{
			while(!inBuffer.isEmpty())
			{
				if(onReceive.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onReceive();
			}
		}
	
	public:
		Delegate<> onReceive;
		
		CInBuffer()
		{	
			typedef CInBuffer<BaseCDevice, length_t, iBufLen> thisclass;
			
			BaseCDevice::onReceive.template bind<thisclass ,& thisclass::getonReceive>(this);
			BaseCDevice::enableonReceive();
		}
		
		void enableonReceive()
		{
			sendonReceive();
		}
				
		void getonReceive()
		{
			char c;
			if(inBuffer.isFull())
			{BaseCDevice::disableonReceive();return;}
			if(BaseCDevice::get(c)) inBuffer.put(c);
			sendonReceive();
		}
		
		/**	\brief	Reads a character from the input buffer
		 *	\param	c	Reference to variable which shall store the character
		 *	\return		true if a character was read
		 */
		
		bool get(char & c)
		{
			bool ret;
			if(inBuffer.isFull())
			{
				typedef CInBuffer<BaseCDevice, length_t, iBufLen> thisclass;
				ret = inBuffer.get(c);
				BaseCDevice::onReceive.template bind<thisclass ,& thisclass::getonReceive>(this);
				BaseCDevice::enableonReceive();
			}else
			{
				ret = inBuffer.get(c);
			}
			return ret;
		}

};
