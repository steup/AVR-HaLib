/**	
 *	
 *	\author	
 *	\date	
 *
 */

#include "avr-halib/avr/interrupt.h"

UseInterrupt(SIG_INTERRUPT3);

class foo
{
public:
 	void dot()
	{
		PORTC ^= 0x80;
	}
};

void jim()
{
	PORTC ^= 0x80;
}

foo hallo;


int main()
{  
	// Buttons (Bit 4-7) als inputs, LEDs (Bit 0-3) als Output
	DDRA = 0x0f;
	DDRB = 0xff;
	DDRC = 0xff;	// Power port
	
	DDRD &= ~0x06;
	PORTD |= 0x06;

	// LEDs anschalten
	PORTA |= 0xf;
	
	{
		//heart beat
		DDRB |= (1<<7);
		TCCR1A = (1 << COM1C0);
		TCCR1B = (1 << CS11)|(1 << CS10);
	}
	EICRA = (1 << ISC31) | (1 << ISC30);		// IRQ on rising edge
	EIMSK = 1 << INT3;				// Enable Interrupts

 	redirectISRM(SIG_INTERRUPT3,&foo::dot, hallo);
	//redirectISRF(SIG_INTERRUPT3, &jim);

	sei();
	
	// LEDs waehrend jeweiligem Button-Druck ausschalten
	while (1)
		PORTA = (PORTA & 0xf0) | (PINA >> 4);
}



// SIGNAL(SIG_INTERRUPT3) { PORTC ^= 0x80; }



