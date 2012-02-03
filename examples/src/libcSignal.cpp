#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect){
    PINA|=0x1;
}

int main(){
    DDRA|=0x1;
    TCCR1B = 1 << CS12;
    TIMSK1 = 1 << TOIE1;
    sei();

    while(true);
    return 0;
}
