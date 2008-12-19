
#include "avr-halib/share/delegate.h"

template <class BaseCDevice, class length_t = uint8_t, length_t oBufLen = 255>
		class outBuffer: public BaseCDevice
{
	private:	//protected:
			QueueBuffer<char, length_t, oBufLen> outBuffer;
	public:
		Delegate onReady;
		void enableonReady(){BaseCDevice::enableonReady();}
		
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
			if(onReady.isEmpty());// eine Aktion notwendig bei leerem delegate
			else onReady(); // möglicherweise mit rückgabe wert versehen
		}
				
		/// Writes a character into the output buffer
		void put(const char c)
		{
			typedef outBuffer<BaseCDevice, length_t, oBufLen> thisclass;
			outBuffer.put(c);
			BaseCDevice::onReady.fromMethod<thisclass ,& thisclass::putonReady>(this);//für Signal anmelden
			BaseCDevice::enableonReady();
		
		}
	
}	

template <class BaseCDevice, class length_t = uint8_t, length_t iBufLen = 255>
		class inBuffer: public BaseCDevice
{
	private:	//protected:
		QueueBuffer<char, length_t, iBufLen> inBuffer;
	public:
		Delegate onRecive;
		
		void enableonRecive()
		{
			BaseCDevice::onRecive.fromMethod<thisclass ,& thisclass::getonRecive>(this);
			BaseCDevice::enableonRecive();
		}
				
		void getonRecive()
		{
			char c;
			BaseCDevice::get(c);
			inBuffer.put(c);
			if(onRecive.isEmpty());// eine Aktion notwendig bei leerem delegate
			else onRecive();
		}
	
		/**	\brief	Reads a character from the input buffer
		 *	\param	c	Reference to variable which shall store the character
		 *	\return		true if a character was read
		 */
		
		bool get(char & c)
		{
			return inBuffer.get(c);
		}

}