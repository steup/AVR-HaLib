/**	
 *	
 *	\author	
 *	\date	
 *
 */

#include <avr/io.h>

#include <avr/interrupt.h>
#include <halib/avr/interrupt.h>

void grun();
class foo
{
	bool on;
public:
	foo() : on (false) {}
 	void dot()
	{
// 		PORTC ^= 0x80;
		if (on)
			PORTC |= 0x80;
		else
			PORTC &= ~0x80;
		on = !on;
	}
	
};

int b;
foo hallo;

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
	EICRA = 1 << ISC31| (1 << ISC30);
	EIMSK = 1 << INT3;
// 	EICRA |= (1 << ISC31);// | (1 << ISC30); //| (1 << ISC21) | (1 << ISC20);	// IRQ on rising edge
// 	EIMSK |= (1 << INT3) ;//| (1 << INT2);					// Enable Interrupts
	grun();
	sei();
	// LEDs waehrend jeweiligem Button-Druck ausschalten
		while (1)
		  PORTA = (PORTA & 0xf0) | (PINA >> 4);
}

void jim(){
// 	hallo.dot();
	PORTC ^= 0x80;
}


//SIGNAL(SIG_INTERRUPT3){PORTC ^= 0x80;}

UseInterrupt(SIG_INTERRUPT3);

GenInterrupt(SIG_INTERRUPT3);
// REDIR_ISR(SIG_INTERRUPT3);


void grun(){

	
	//redirectISR(SIG_INTERRUPT3,&foo::dot,&hallo);
	//redirectISR(SIG_INTERRUPT3,&hallo.dot,&hallo);
// 	redirectISR(SIG_INTERRUPT3, foo, dot, &hallo);
	
	redirectISRMF(SIG_INTERRUPT3,&foo::dot, hallo);
	//redirectISRF(SIG_INTERRUPT3, &jim);
	
	
}
