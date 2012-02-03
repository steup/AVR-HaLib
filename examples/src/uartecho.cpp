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
#include "avr-halib/share/coutdevice.h"
#include "avr-halib/share/cbuffer.h"
#include "avr-halib/share/queuebuffer.h"


int main()
{
	COutDevice< SecOut<Uartnoint<Uart1<> > > >uart;
// 	CInDevice< QueueBuffer<char, uint8_t, 200> > buffer;
	union {
		char c;
		unsigned char uc;
		uint8_t b;
		uint8_t ub;
		int16_t w;
		uint16_t uw;
		int32_t d;
		uint32_t ud;
		uint32_t data;
	}in;
	in.data=0;
	// enable interrupts
	sei();

	uart << "echos every char";
	uart.writeNewline();
	
	while(1)
	{
		if (uart.get(in.c))
		{
			uart << in.c; //display input 
			BASE<16>(uart) << "\t\t" << in.data; // echo buffer integer as hexadecimal
			BASE<256>(uart) << "\t\t" << in.data;// echo buffer integer as  char
			uart.writeNewline();
			uart << "\t" << in.c << "\t"<< in.uc << "\t"<< in.b<< "\t"<< in.ub<< "\t"<< in.w<< "\t"<< in.uw << "\t"<< in.d<< "\t"<< in.ud ; // echo interprete by type (uart declared without base -> Base =0)
			uart.writeNewline();
			BASE<0>(uart) <<"base 0:"<< "\t" << in.c << "\t"<< in.uc << "\t"<< in.b<< "\t"<< in.ub<< "\t"<< in.w<< "\t"<< in.uw << "\t"<< in.d<< "\t"<< in.ud ; // echo interprete by type
			uart.writeNewline();
			DEC(uart) <<"dec:"<< "\t" << in.c << "\t"<< in.uc << "\t"<< in.b<< "\t"<< in.ub<< "\t"<< in.w<< "\t"<< in.uw << "\t"<< in.d<< "\t"<< in.ud; // echo interprete integer as decimal
			uart.writeNewline();
			CHAR(uart) <<"char:"<< "  " << in.c << "\t"<< in.b<< "\t"<< in.w<< "\t"<< in.d ; // echo interprete integer as char (singned or unsigned are interpreted the same way)
			HEX(uart) <<"\thex:"<< " " << in.c << "\t"<< in.b<< "\t"<< in.w<<  "\t"<< in.d; // echo interprete integer as hexadecimal(singned or unsigned are interpreted the same way)
			uart.writeNewline();
#if 0 
			//aktivate if you want to see that singned and unsigned are interpreted the same way for HEX and CHAR
			CHAR(uart) <<"char:"<< "  " << in.uc << "\t"<< in.ub<< "\t"<< in.uw<< "\t"<< in.ud ; // echo interprete integer as char (singned or unsigned are interpreted the same way)
			HEX(uart) <<"\thex:"<< " " << in.uc << "\t"<< in.ub<< "\t"<< in.uw<<  "\t"<< in.ud; // echo interprete integer as hexadecimal(singned or unsigned are interpreted the same way)
			uart.writeNewline();
#endif
			in.data <<= 8; // shift data to fill uint32_t data buffer
			uart.writeNewline();
		}
	}
}

