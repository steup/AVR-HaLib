/**
 *  \file  examples/application/led.cpp
 *  \brief Example illustrating usage of Led
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */

#include "platform.h"
#include <avr-halib/ext/led.h>
#include <avr-halib/common/delay.h>

using avr_halib::ext::Led;

int main()
{
  Led< platform::Led0 > led0;

  led0.setOn();
  delay_ms(1000);
  led0.setOff();
  delay_ms(1000);
  if(!led0.isOn())
    led0.toggle();

  while (1);
  return 0;
}
