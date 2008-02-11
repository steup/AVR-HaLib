/**	
 *	Testprogramm fuer  auf Robby-Board
 *	
 *	\author	Philipp Werner
 *	\date	29.10.2007
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
int main()
{  
	// Buttons (Bit 4-7) als inputs, LEDs (Bit 0-3) als Output
	DDRA = 0x0f;
	DDRB = 0xff;
	DDRC = 0xff;
	
	MCUCR &= ~(1<<PUD);
	
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
	EICRA = 1 << ISC31;
	EIMSK = 1 << INT3;
// 	EICRA |= (1 << ISC31);// | (1 << ISC30); //| (1 << ISC21) | (1 << ISC20);	// IRQ on rising edge
// 	EIMSK |= (1 << INT3) ;//| (1 << INT2);					// Enable Interrupts
	sei();
	// LEDs waehrend jeweiligem Button-Druck ausschalten
		while (1)
		  PORTA = (PORTA & 0xf0) | (PINA >> 4);
}
SIGNAL(SIG_INTERRUPT3)
{
	PORTC ^= 0x80;
}
