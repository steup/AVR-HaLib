#ifdef __AVR_AT90CAN128__
#define CPU_FREQUENCY 16000000UL
#else
#define CPU_FREQUENCY 8000000UL
#endif


#define F_CPU CPU_FREQUENCY
//#define UART_BAUDRATE 19200
#include "halib/avr/uart.wip.h"
#include <util/delay.h>


int main()
{
	Uart<Uart1> uart;
	
	// enable interrupts
	sei();
//	char c;
//  	char* c="hallo\n\r";
// 	int i=0;
	
	while(1)
	{
		char c2;
		if((c2 = uart.getc()) != 0) uart.putc(c2);
// 		_delay_ms(10);
//  		uart.putc(c[i++]);
//  		i=i%7;
	}
}

extern "C" void __cxa_pure_virtual() {}
