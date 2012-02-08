/**
 *	\file	examples/application/uart.cpp
 *	\brief	Example illustrating usage of Uart
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "avr-halib/avr/uart.h"

typedef avr_halib::drivers::Uart::configure<>::type Uart;

Uart uart;

int main(){
	while(true){
        char c;
        if(uart.get(c)){
            uart.put('\n');
            uart.put(c);
        }
	}
    return 0;
}
