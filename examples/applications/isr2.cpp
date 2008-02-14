/**	
 *	
 *	\author	
 *	\date	
 *
 */

#include <avr/io.h>

#include <avr/interrupt.h>
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

// macro for setting the redirection function 
// for a specific vector
#define redirectISR(vector,func,object)	__redirectISR(vector,func,object)
#define __redirectISR(vector,func,object)			\
{							\
	extern  void (vector ## _TYPE::*vector ## _REDIR_FUNCTION)() ;		\
	extern vector ## _TYPE * vector ## _REDIR_OBJECT;\
	uint8_t tmp=SREG;				\
	cli();						\
	vector ## _REDIR_FUNCTION=(void (vector ## _TYPE::*)())func;		\
	vector ## _REDIR_OBJECT=(vector ## _TYPE *)object;	\
	SREG=tmp;					\
}

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

public:
    delegate() : obj_ptr_( 0 ), stub_ptr_( 0 ) { }

    template<typename T, void (T::*Fxn)()>
    void from_function(T * obj)
    {
        obj_ptr_ = const_cast<T const *>( obj );
        stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
    }

    template<typename T, void (T::*Fxn)() const>
    void from_function(T const * obj)
    {
        obj_ptr_ = obj;
        stub_ptr_ = &mem_fn_const_stub<T, Fxn>::invoke;
    }

    template<void (*Fxn)()>
    void from_function()
    {
        obj_ptr_ = 0;
        stub_ptr_ = &function_stub<Fxn>::invoke;
    }

    void operator ()() const
    {
        ( *stub_ptr_ )( obj_ptr_);
    }
};

//template <class T>
struct foobar
{
    void foo() { printf("%s\n",__PRETTY_FUNCTION__);}
    void bar() const { printf("%s\n",__PRETTY_FUNCTION__);}
};

void hello() { printf("%s\n",__PRETTY_FUNCTION__);}

delegate dg, &d=dg;

int main()
{
    foobar fb;
    foobar *pfb = &fb;


    dg.from_function<foobar, &foobar::foo>( pfb );
    dg(); // (pfb->*&foobar::foo)( 1 );


    dg.from_function<foobar , &foobar::bar>( pfb );
    d(); // (pfb->*&foobar::bar)( 1 );


    dg.from_function<&hello>();
    d(); // hello( 1 );

return 0;
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
	//redirectISR(SIG_INTERRUPT3,&hallo.dot,&hallo);

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
