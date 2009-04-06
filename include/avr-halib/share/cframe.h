#pragma once
#include "avr-halib/share/queuebuffer.h"
#include "avr-halib/share/delegate.h"

template <class BaseCDevice, class framelength_t, class length_t = uint8_t, length_t BufLen = 255>
		class CFrame: public BaseCDevice
{
	private:	
	//protected:
		typedef CFrame<BaseCDevice,framelength_t,length_t,BufLen> thisclass;
			
		enum Framestate
		{ wait, len, regular, stuff};
		
		struct Frame
		{	
			framelength_t length;
			char data[BufLen];
			framelength_t pos;
			Framestate state;
		};
		
		Frame	inFrame ;
		Frame	outFrame;
		
		
// 		enum{esc=0x1b,sofr=0x0a,eofr=0xd};
		enum{esc='e',sofr='a',eofr='s'};
		
		void sendonReady()
		{
			while(outFrame.length == 0)
				if(onReady.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onReady();
		}
		
		void sendonRecive()
		{
			while(inFrame.pos < inFrame.length )
			{
				if(onRecive.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onRecive();
			}
		}
	public:
		Delegate onReady;
		Delegate onRecive;
		
		CFrame()
		{	
			
			BaseCDevice::onRecive.template fromMethod<thisclass ,& thisclass::getonRecive>(this);
			BaseCDevice::enableonRecive();
		}
		
		void enableonReady(){
			
// 			BaseCDevice::activateonReady();
			sendonReady();
		}
		
		void enableonRecive()
		{
			sendonRecive();
// 			BaseCDevice::onRecive.template fromMethod<thisclass ,& thisclass::getonRecive>(this);
// 			BaseCDevice::activateonRecive();
		}
		
		// forwards char on BaseCDevice ready
		void putonReady(){
			//hier wird gestufft
			if( outFrame.pos < outFrame.length )
			{	
				if(outFrame.pos==0 && outFrame.state == wait)
				{
					BaseCDevice::put(sofr);
					outFrame.state = regular;
				}else
				{
					char c = outFrame.data[outFrame.pos];
	 				if( c == esc || c == sofr || c == eofr)
					{	
						if( outFrame.state == stuff )
						{
							BaseCDevice::put(c);
							outFrame.state=regular;
							outFrame.pos++;
						}else
						{
							BaseCDevice::put(esc);
							outFrame.state=stuff;
						}
					}else
					{
						BaseCDevice::put(c);
						outFrame.pos++;
					}
				}
			}
			else
			{	
				BaseCDevice::put(eofr);
				outFrame.state = wait;
				outFrame.length = 0;
				sendonReady();
				BaseCDevice::disableonReady();
			}
			
		}
				
		// Writes a character into the output buffer
		void send(char* data, framelength_t len)
		{
 			
			for(framelength_t i = 0;i < len; i++)
				outFrame.data[i] = data[i];
			outFrame.length = len;
			outFrame.pos = 0;
			
			BaseCDevice::onReady.template fromMethod< thisclass , &thisclass::putonReady >(this);//für Signal anmelden
			BaseCDevice::enableonReady();
		
		}
				
		void getonRecive()
		{
			//hier wird destufft
			char c;
			BaseCDevice::get(c);
			if( inFrame.state == stuff )
			{
				inFrame.data[inFrame.length]=c;
				inFrame.state=regular;
				inFrame.length++;
				return;
			}
			if( inFrame.state == wait && c != sofr)
			{
				return;
			}
			if(inFrame.state != stuff)
			{
				if(c == sofr)
				{
					inFrame.length=0;
					inFrame.pos=0;
					inFrame.state=regular;
					return;
				}
				if( c == eofr)
				{
					inFrame.state=wait;
					sendonRecive();
					return;
				}
				if( c == esc)
				{
					inFrame.state=stuff;
					return;
				}
				inFrame.data[inFrame.length]=c;
				inFrame.length++;
			}
			
		}

		
		/**	\brief	Reads a character from the input buffer
		 *	\param	c	Reference to variable which shall store the character
		 *	\return		true if a character was read
		 */
		
		bool recive(char* data, framelength_t & len)
		{
			if(inFrame.state == wait && inFrame.pos < inFrame.length && len >= inFrame.length)
			{
				for( inFrame.pos = 0; inFrame.pos < inFrame.length ;inFrame.pos++)
					data[inFrame.pos] = inFrame.data[inFrame.pos];
				len = inFrame.length;
				return true;
			}
			return false;
			
		}
		
};	

