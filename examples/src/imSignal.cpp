#include <avr-halib/avr/InterruptManager/InterruptManager.h>
#include <avr-halib/avr/InterruptManager/SignalSemanticInterrupt.h>
#include <boost/mpl/vector.hpp>
#include <avr/io.h>

using boost::mpl::vector;
using Interrupt::Slot;
using Interrupt::Binding;
using Interrupt::InterruptManager;


IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(blink)
{
    PINA|=0x1;
}


typedef vector< Slot< 15, Binding::SignalSemanticFunction >::Bind< &blink > >::type Interrupts;

int main()
{
    DDRA|=0x1;
    InterruptManager<Interrupts>::init();

    TCCR1B = 1 << CS12;
    TIMSK1 = 1 << TOIE1;
    asm("sei");

    while(true);
    return 0;
}
