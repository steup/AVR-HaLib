#pragma once
#include "avr-halib/share/queuebuffer.h"
#include "avr-halib/share/delegate.h"

template <class BaseCDevice, class length_t = uint8_t, length_t oBufLen = 255>
		class outBuffer: public BaseCDevice
{
	private:	
	//protected:
		QueueBuffer<char, length_t, oBufLen> outBuffer;
		void sendonReady()
		{
			while(!outBuffer.isFull)
				if(onReady.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onReady();onReady();
		}
	public:
		Delegate onReady;
		void activateonReady(){
			
			BaseCDevice::activateonReady();
			sendonReady();
		}
		
		/// forwards char on BaseCDevice ready
		void putonReady(){
			char c;
			if (outBuffer.isEmpty())
			{
				//BaseCDevice::disableonReady(); statt disable reset erkennen(delegate is empty)
				BaseCDevice::onReady.reset();
			}
			else 
			{
				outBuffer.get(c);
				BaseCDevice::put(c);
			}
			sendonReady();
		}
				
		/// Writes a character into the output buffer
		void put(const char c)
		{
// 			typedef typeof(this)outBuffer<BaseCDevice, length_t, oBufLen> thisclass;
			typedef typeof(outBuffer<BaseCDevice, length_t, oBufLen>) thisclass;
			
			outBuffer.put(c);
			BaseCDevice::onReady.template fromMethod< thisclass , &thisclass::putonReady >(this);//für Signal anmelden
			BaseCDevice::activateonReady();
		
		}
	
};	

template <class BaseCDevice, class length_t = uint8_t, length_t iBufLen = 255>
		class inBuffer: public BaseCDevice
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
		
		void activateonRecive()
		{
			typedef typeof (inBuffer<BaseCDevice, length_t, iBufLen>) thisclass;
			
			sendonRecive();
			BaseCDevice::onRecive.template fromMethod<thisclass ,& thisclass::getonRecive>(this);
			BaseCDevice::activateonRecive();
		}
				
		void getonRecive()
		{
			char c;
			if(BaseCDevice::get(c))inBuffer.put(c);
			sendonRecive();
		}
		
		/**	\brief	Reads a character from the input buffer
		 *	\param	c	Reference to variable which shall store the character
		 *	\return		true if a character was read
		 */
		
		bool get(char & c)
		{
			return inBuffer.get(c);
		}

};
