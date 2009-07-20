#pragma once
#include "avr-halib/share/delegate.h"

struct CFrameConf
{
// 		enum {esc=0x1b,sofr=0x0a,eofr=0xd, esc_modifier=0x20};
		enum {esc='e',sofr='a',eofr='s', escmod=0x01};
};

template <class BaseCDevice, class framelength_t = uint8_t, class cframeconf = struct CFrameConf,framelength_t BufLen = 255>
		class CFrame: public BaseCDevice
{
	private:	
	//protected:
		typedef CFrame<BaseCDevice,framelength_t,cframeconf,BufLen> thisclass;
			
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
		
		typedef cframeconf ctrchars;
		
		void sendonReady()
		{
			while(outFrame.length == 0)
				if(onReady.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onReady();
		}
		
		void sendonReceive()
		{
			while(inFrame.pos < inFrame.length )
			{
				if(onReceive.isEmpty())break;// keine Aktion notwendig bei leerem delegate
				else onReceive();
			}
		}
	public:
		Delegate<> onReady;
		Delegate<> onReceive;
		
		CFrame()
		{	
			
			BaseCDevice::onReceive.template bind<thisclass ,& thisclass::getonReceive>(this);
			BaseCDevice::enableonReceive();
		}
		
		void enableonReady(){
			
			sendonReady();
		}
		
		void enableonReceive()
		{
			sendonReceive();
		}
		
		// forwards char on BaseCDevice ready
		void putonReady(){
			//hier wird gestufft
			if( outFrame.pos < outFrame.length )
			{	
				if(outFrame.pos==0 && outFrame.state == wait)
				{
					BaseCDevice::put(ctrchars::sofr);
					outFrame.state = regular;
				}else
				{
					char c = outFrame.data[outFrame.pos];
					if( c == ctrchars::esc || c == ctrchars::sofr || c == ctrchars::eofr)
					{	
						if( outFrame.state == stuff )
						{
							BaseCDevice::put((ctrchars::escmod!=0)?(c^ctrchars::escmod):c);
							outFrame.state=regular;
							outFrame.pos++;
						}else
						{
							BaseCDevice::put(ctrchars::esc);
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
				BaseCDevice::put(ctrchars::eofr);
				outFrame.state = wait;
				outFrame.length = 0;
				sendonReady();
				BaseCDevice::disableonReady();
			}
			
		}
		
		// Writes a character into the output buffer
		bool send(char* data, framelength_t len)
		{
			if(outFrame.length > 0 ) return false;
			for(framelength_t i = 0;i < len; i++)
				outFrame.data[i] = data[i];
			outFrame.length = len;
			outFrame.pos = 0;
			
			BaseCDevice::onReady.template bind< thisclass , &thisclass::putonReady >(this);//für Signal anmelden
			BaseCDevice::enableonReady();
			return true;
		}
		
		void getonReceive()
		{
			//hier wird destufft
			char c;
			BaseCDevice::get(c);
			if( inFrame.state == stuff )
			{
				inFrame.data[inFrame.length]=(ctrchars::escmod!=0)?(c^ctrchars::escmod):c;
				inFrame.state=regular;
				inFrame.length++;
				return;
			}
			if( inFrame.state == wait && c != ctrchars::sofr)
			{
				return;
			}
			if(inFrame.state != stuff)
			{
				if(c == ctrchars::sofr)
				{
					inFrame.length=0;
					inFrame.pos=0;
					inFrame.state=regular;
					return;
				}
				if( c == ctrchars::eofr)
				{
					inFrame.state=wait;
					sendonReceive(); /// send interupt
					return;
				}
				if( c == ctrchars::esc)
				{
					inFrame.state=stuff;
					return;
				}
				inFrame.data[inFrame.length]=c;
				inFrame.length++;
			}
			
		}

		
		
		
		/**	\brief	Reads Frame
		 *	\param	c	Reference to variable which shall store the character
		 *	\return		true if a character was read
		 */
		
		bool receive(char* data, framelength_t & len)
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

