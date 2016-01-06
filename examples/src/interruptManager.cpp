/*******************************************************************************
 *
 * Copyright (c) 2010-2011 Michael Schulze <mschulze@ivs.cs.uni-magdeburg.de>
 * All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions
 *    are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *
 *    * Neither the name of the copyright holders nor the names of
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * $Id: interruptManager.cpp 756 2011-12-06 23:18:29Z mschulze $
 *
 ******************************************************************************/

#include "platform.h"

#undef _VECTOR
#define _VECTOR(x) x
#include <avr/io.h>
#include <avr/interrupt.h>
#include <boost/mpl/vector.hpp>

#include "avr-halib/interrupts/InterruptManager/InterruptManager.h"

void function2() {
    log::emit() << "FPF"<<log::endl;
}

#include "avr-halib/interrupts/InterruptManager/SignalSemanticInterrupt.h"
IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(function){
    log::emit() << "SSF"<<log::endl;
}

struct InterruptConfig
{
    typedef boost::mpl::vector<
              avr_halib::interrupts::interrupt_manager::Slot<TIMER1_COMPA_vect,avr_halib::interrupts::interrupt_manager::Binding::SignalSemanticFunction>::Bind<&function>,
                avr_halib::interrupts::interrupt_manager::Slot<TIMER1_COMPB_vect,avr_halib::interrupts::interrupt_manager::Binding::FixedPlainFunction>::Bind<&function2>,
                avr_halib::interrupts::interrupt_manager::Slot<TIMER1_OVF_vect,avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction>::Bind
                ,avr_halib::interrupts::interrupt_manager::DefaultSlot< avr_halib::interrupts::interrupt_manager::Binding::FixedPlainFunction>::Bind<&function2>::type
            >::type config;
};

// providing the InterruptManager with the needed information and switching to
// non debug mode represented by the false flag given as second template
// parameter
typedef avr_halib::interrupts::interrupt_manager::InterruptManager<InterruptConfig::config> IM;

BIND_INTERRUPTS(IM);

struct Test
{
    static void tick()
    {
        log::emit() << "tick"<<log::endl;
        // rebind the overflow interrupt dynamically
        IM::bind<avr_halib::interrupts::interrupt_manager::Slot<TIMER1_OVF_vect>,&Test::tack>();
    }
    static void tack()
    {
        log::emit() << "tack"<<log::endl;
        // rebind the overflow interrupt dynamically
        IM::bind<avr_halib::interrupts::interrupt_manager::Slot<TIMER1_OVF_vect>,&Test::tick>();
    }
};

int main(void) {
    log::emit() << "Hallo World"<<log::endl;

    // bind the overflow interrupt dynamically
    IM::bind<avr_halib::interrupts::interrupt_manager::Slot<TIMER1_OVF_vect>,&Test::tick>();

    log::emit() << "initialize the timer 1" << log::endl;
    TCCR1B = ((1<<CS12) | (1<<CS10));   // 1024 prescaler = 4s
    OCR1A  = 22000;
    OCR1B  = 44000;
    TIMSK1 = (1<<OCIE1A) | (1<<OCIE1B) | (1<<TOIE1); // enable irqs

    log::emit() << "allow interrupts globally"<<log::endl;
    sei(); // enable global interrupts
    while(1);
}
