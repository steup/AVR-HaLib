/**
 *  \file  examples/applications/button.cpp
 *  \brief  Example for usage of Button and Led (very simple)
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */

#include "platform.h"

#include "avr-halib/drivers/ext/button.h"
#include "avr-halib/drivers/ext/led.h"

using avr_halib::drivers::ext::Button;
using avr_halib::drivers::ext::Led;

int main()
{
    Button< platform::Button0 > button0;
    Led   < platform::Led0    > led0;
    Led   < platform::Led1    > led1;
    Led   < platform::Led2    > led2;
  
    while (1)
    {
        if(button0.isPressed())
        {
            led0.set(1);
            led1.set(1);
            led2.set(1);
        }
        else
        {
            led0.set(0);
            led1.set(0);
            led2.set(0);
        }
    }
}
