/**
 *  \file  examples/application/uart.cpp
 *  \brief  Example illustrating usage of Uart
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */

#include "avr-halib/drivers/avr/uart.h"

struct Config : public avr_halib::drivers::avr::Uart::DefaultConfig
{
    typedef avr_halib::regmaps::local::Uart1 RegMap;
};

typedef avr_halib::drivers::avr::Uart::configure<Config>::type Uart;

Uart uart;

int main(){
    uint8_t c;
    while(true){
        if(uart.get(c)){
            uart.put('\n');
            uart.put(c);
        }
    }
    return 0;
}
