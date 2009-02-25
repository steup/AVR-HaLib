/**
 *	\file	examples/application/uart.cpp
 *	\brief	Example illustrating usage of Uart, CInDevice and COutDevice
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#ifdef __AVR_AT90CAN128__
#define CPU_FREQUENCY 16000000UL
#else
#define CPU_FREQUENCY 8000000UL
#endif


#include "avr-halib/avr/uart.h"
#include "avr-halib/share/cdevice.h"
#include "avr-halib/share/queuebuffer.h"

UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);



int main()
{
	COutDevice< Uart<Uart1<> > > uart;
	CInDevice< QueueBuffer<char, uint8_t, 200> > buffer;
	char c = 0;
	
	// enable interrupts
	sei();

	uart << "For testing string and integer functions of CDevice enter 's' or 'i'";
	uart.writeNewline();
	
	while(1)
	{
		if (uart.get(c))
			uart.put(c);
		
		if (c == 'i')
		{
			uart.writeNewline();
			uart << "Enter integer, start parsing with '!' ... ";
			
			while (c != '!')
			{
				if (uart.get(c))
				{
					uart.put(c);
					buffer.put(c);
				}
			}
			uart.writeNewline();
		
			int32_t n;
			if (buffer.readInt(n))
				uart << " [ Number: " << n << " ] ";
			else
				uart << " [ No number! ]";
			
			buffer.clear();
		}
		else if (c == 's')
		{
			uart.writeNewline();
			uart << "Enter string, start parsing with '!' ... ";
			
			while (c != '!')
			{
				if (uart.get(c))
				{
					uart.put(c);
					if (c != '!')
						buffer.put(c);
				}
			}
			uart.writeNewline();


			char s [255];
			uart << " [ String: ";
			while (buffer.readString(s, 255))
				uart << "\"" << s << "\" ";
			uart << " ]";
		
			buffer.clear();
		}
	}
}

