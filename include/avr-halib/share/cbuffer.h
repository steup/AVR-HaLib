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
		Delegate onReady;
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
// 			typedef typeof(this)outBuffer<BaseCDevice, length_t, oBufLen> thisclass;
			typedef COutBuffer<BaseCDevice, length_t, oBufLen> thisclass;
// 			typedef typeof(this) thisclass;
			
			outBuffer.put(c);
			BaseCDevice::onReady.template fromMethod< thisclass , &thisclass::putonReady >(this);//für Signal anmelden
// 			BaseCDevice::onReady.template fromMethod< outBuffer<BaseCDevice, length_t, oBufLen>  , &outBuffer<BaseCDevice, length_t, oBufLen>::putonReady >(this);//für Signal anmelden
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
		void sendonRecive()
		{
			while(!inBuffer.isEmpty())
			{
				if(onRecive.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onRecive();
			}
		}
	
	public:
		Delegate onRecive;
		
		CInBuffer()
		{	
			typedef CInBuffer<BaseCDevice, length_t, iBufLen> thisclass;
			
			BaseCDevice::onRecive.template fromMethod<thisclass ,& thisclass::getonRecive>(this);
			BaseCDevice::enableonRecive();
		}
		
		void enableonRecive()
		{
			sendonRecive();
// 			BaseCDevice::onRecive.template fromMethod<thisclass ,& thisclass::getonRecive>(this);
// 			BaseCDevice::activateonRecive();
		}
				
		void getonRecive()
		{
			char c;
			if(inBuffer.isFull())
			{BaseCDevice::disableonRecive();return;}
			if(BaseCDevice::get(c)) inBuffer.put(c);
			sendonRecive();
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
				ret = inBuffer.get(c);
				BaseCDevice::enableonRecive();
			}else
			{
				ret = inBuffer.get(c);
			}
			return ret;
		}

};
