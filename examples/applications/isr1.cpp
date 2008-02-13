/**	
 *	
 *	\author	
 *	\date	
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "interrupt.h"
void grun();
class foo
{
	bool on;
public:
	foo() : on (false) {}
 	void dot()
	{
//		PORTC ^= 0x80;
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
	EICRA = 1 << ISC31;
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
	hallo.dot();
// 	PORTC ^= 0x80;
}




// #define REDIR_ISR(vector)	__REDIR_ISR(vector)
// #define __REDIR_ISR(vector) 				\
// 	class vector ## _TYPE{};						\
// 	void (vector ## _TYPE::*vector ## _REDIR_FUNKTION)() =0;			\
// 	uint16_t vector ## _REDIR_OBJECT=0;			
// 
// #define redirectISR(vector,func,object)	__redirectISR(vector,func,object)
// #define __redirectISR(vector,func,object)			\
// do {							\
// 	extern  void (vector ## _TYPE::*vector ## _REDIR_FUNKTION)() ;		\
// 	extern uint16_t vector ## _REDIR_OBJECT;\
// 	uint8_t tmp=SREG;				\
// 	cli();						\
// 	vector ## _REDIR_FUNKTION=(void (vector ## _TYPE::*)())func;		\
// 	vector ## _REDIR_OBJECT=(uint16_t)object;	\
// 	SREG=tmp;					\
// } while(0)

// #define REDIR_ISR(vector)	__REDIR_ISR(vector)
// #define __REDIR_ISR(vector) 				\
// 	class vector ## _TYPE{};						\
// 	void (vector ## _TYPE::*vector ## _REDIR_FUNCTION)() =0;			\
// 	vector ## _TYPE * vector ## _REDIR_OBJECT=0;			
// 
// #define redirectISR(vector,func,object)	__redirectISR(vector,func,object)
// #define __redirectISR(vector,func,object)			\
// do {							\
// 	extern  void (vector ## _TYPE::*vector ## _REDIR_FUNCTION)() ;		\
// 	extern vector ## _TYPE * vector ## _REDIR_OBJECT;\
// 	uint8_t tmp=SREG;				\
// 	cli();						\
// 	vector ## _REDIR_FUNCTION=(void (vector ## _TYPE::*)())func;		\
// 	vector ## _REDIR_OBJECT=(vector ## _TYPE *)object;	\
// 	SREG=tmp;					\
// } while(0)


REDIR_ISR(SIG_INTERRUPT3);


void grun(){

	
	redirectISR(SIG_INTERRUPT3,&foo::dot,&hallo);

}
		
typedef void (foo::*foofunct)();


// SIGNAL(SIG_INTERRUPT3)
// {
// // 	PORTC ^= 0x80;
// // 	jim();
// 	foo * g = (foo*)gruneTomaten_REDIR_OBJECT;
// 	foofunct f = (foofunct)gruneTomaten_REDIR_FUNCTION;
//  	(g->*f)();
// //  	((foo*)gruneTomaten_REDIR_OBJECT)->*((void (foo::*)())gruneTomaten_REDIR_FUNCTION)();
// }
