
#include <avr/io.h>


#define F_CPU 16000000

#include "halib/share/delay.h"



int main()
{
	DDRA |= 1;
	while (1)
	{
		PORTA = ((PORTA & 1) ^ 1) | (PORTA & ~1);
		delay_ms(1000);
	}
}
