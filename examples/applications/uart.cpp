#ifdef __AVR_AT90CAN128__
#define CPU_FREQUENCY 16000000UL
#else
#define CPU_FREQUENCY 8000000UL
#endif


#include "halib/avr/uart.h"
#include "halib/share/cbuffer.h"


int main()
{
	Uart<Uart1> uart;
	CBuffer<uint8_t, 200> buffer;
	char c;
	
	// enable interrupts
	sei();

	uart << "For testing string and integer functions of CDevice enter 's' or 'i'";
	uart.newline();
	
	while(1)
	{
		if (uart.getc(c))
			uart.putc(c);
		
		if (c == 'i')
		{
			uart.newline();
			uart << "Enter integer, start parsing with '!' ... ";
			
			while (c != '!')
			{
				if (uart.getc(c))
				{
					uart.putc(c);
					buffer.putc(c);
				}
			}
			uart.newline();
		
			int32_t n;
			if (buffer.readInt(n))
				uart << " [ Number: " << n << " ] ";
			else
				uart << " [ No number! ]";
			
			buffer.clear();
		}
		else if (c == 's')
		{
			uart.newline();
			uart << "Enter string, start parsing with '!' ... ";
			
			while (c != '!')
			{
				if (uart.getc(c))
				{
					uart.putc(c);
					if (c != '!')
						buffer.putc(c);
				}
			}
			uart.newline();


			char s [255];
			uart << " [ String: ";
			while (buffer.readString(s, 255))
				uart << "\"" << s << "\" ";
			uart << " ]";
		
			buffer.clear();
		}
	}
}

