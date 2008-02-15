/**	
 *	
 *	\author	
 *	\date	
 *
 */

#include <avr/io.h>

#include <avr/interrupt.h>

template<typename T, void (T::*Fxn)()>
		struct mem_fn_stub
{
	static void invoke(void const * obj_ptr)
	{
		T * obj = static_cast<T *>( const_cast<void *>( obj_ptr ) );
		(obj->*Fxn)();
	}
};

template<typename T, void (T::*Fxn)() const>
		struct mem_fn_const_stub
{
	static void invoke(void const * obj_ptr)
	{
		T const * obj = static_cast<T const *>( obj_ptr );
		(obj->*Fxn)();
	}
};

template<void (*Fxn)()>
		struct function_stub
{
	static void invoke(void const *)
	{
		(*Fxn)( );
	}
};



// defining of redirection memory variables and 
// the redirection stub for the certain vector

// extern "C" void _redir_func();

#define REDIR_ISR(vector)	__REDIR_ISR(vector)
#define __REDIR_ISR(vector) 				\
	typedef void (*invoke_stub)(void const *);\
    	void const  *vector ## _REDIR_OBJECT;	\
       	invoke_stub vector ## _REDIR_FUNCTION;						\
	void vector (void) __attribute__ ((naked)); 	\
	void vector (void) {				\
		asm volatile (				\
			"push r31 		\n"	\
			"push r30 		\n"	\
			"lds r30, %[Function] 	\n"	\
			"lds r31, %[Function]+1	\n"	\
			"push r25 		\n"	\
			"push r24 		\n"	\
			"lds r24, %[Obj] 		\n"	\
			"lds r25, %[Obj]+1		\n"	\
			"jmp redir_func 	\n"  	\
			:  				\
			:[Function] "m" (vector ## _REDIR_FUNCTION),[Obj] "m" (vector ## _REDIR_OBJECT)\
			);				\
		}

#define REDIR_ISR_TEMPLATES(vector) __REDIR_ISR_TEMPLATES(vector)
#define __REDIR_ISR_TEMPLATES(vector)			\
	typedef void (*invoke_stub)(void const *); \
	extern void const  *vector ## _REDIR_OBJECT;	\
	extern invoke_stub vector ## _REDIR_FUNCTION;	\
	template<typename T, void (T::*Fxn)()> \
	void vector ## _from_function(T * obj) \
	{\
		vector ## _REDIR_OBJECT = const_cast<T const *>( obj );\
		vector ## _REDIR_FUNCTION = &mem_fn_stub<T, Fxn>::invoke;\
	};\
	\
	template<typename T, void (T::*Fxn)() const> \
	void vector ## _from_function(T const * obj) \
	{\
		vector ## _REDIR_OBJECT = obj;\
		vector ## _REDIR_FUNCTION = &mem_fn_const_stub<T, Fxn>::invoke;\
	};\
	\
	template<void (*Fxn)()> \
	void vector ## _from_function() \
	{\
		vector ## _REDIR_OBJECT = 0;\
		vector ## _REDIR_FUNCTION = &function_stub<Fxn>::invoke;\
	};\


// macro for setting the redirection function 
// for a specific vector
#define redirectISR(vector, fclass, func, object)	__redirectISR(vector, fclass, func, object)
#define __redirectISR(vector, fclass, func, object)			\
{							\
	uint8_t tmp=SREG;				\
	cli();						\
	vector ## _from_function<fclass, &fclass::func>(&object); \
	SREG=tmp;					\
}



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






REDIR_ISR(SIG_INTERRUPT3);

REDIR_ISR_TEMPLATES(SIG_INTERRUPT3)


void grun(){

	
	//redirectISR(SIG_INTERRUPT3,&foo::dot,&hallo);
	//redirectISR(SIG_INTERRUPT3,&hallo.dot,&hallo);
	redirectISR(SIG_INTERRUPT3, foo, dot, hallo);
	
}
