#define CPU_FREQUENCY 8000000UL
#define F_CPU CPU_FREQUENCY
#define UART_BAUDRATE 19200
#include "halib/uart.h"
#include <util/delay.h>
int main()
{
	Uart<uint8_t, 255, 20> uart;
	
// 	char c;
	char* c="hallo";
	int i=0;
	
	while(1)
	{
// 		if((c = uart.getc()) != 0) uart.putc(c);
		
		uart.putc(c[i++]);
		i=i%6;
	}
}