/* 
 * Shumway, a CAN-bootloader for Atmel AVR microcontoller. 
 * Copyright (C) 2006 Joerg Diederich, Dipl. Inform. Michael Schulze 
 * Embedded Systems and Operating Systems 
 * Department of Distributed Systems (IVS)
 * Faculty of Computer Science
 * Otto-von-Guericke-University Magdeburg, Germany
 *
 * joerg.diederich@graffiti.net
 * mschulze@ivs.cs.uni-magdeburg.de 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either version 2 
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, 
 * USA. 
 */

/*$Id: main.cpp 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file main.cpp
 * \brief Program start.
 *
 * Main of the Shumway programmer. Includes the entry point for another
 * application as well.
 */

/*!
 *\mainpage 
 * 
 * Your are wachting the documentation of the Shumway programmer, a programmer
 * for Atmel AVR microcontrollers connected via a CAN bus. \n
 * \n
 * For usage, an cusomised version of avrdude is available. \n
 * \n
 * This  documentation was generated using the Doxygen documentation generator.
 * \n
 * Required software: 
 * \li avr-gcc 4.1.1
 * \li avr-libc 1.4.4 or above
 * \li A microcontroller programmer, e.g. avrdude together with a stk.
 *
 * Installation: \n
 * \li Change path to compiler in the provided Makefile. 
 * \li Compile using the provided Makefile. 
 * \li Write bootloader to microcontroller with programmer.
 * 
 * Usage: \n
 * \li Connect microcontroller with installed Shumway to CAN bus.
 * \li Use custom programmer software to connect to the microcontroller. 
 * In case of using the customised version of avrdude, the option
 * '-c pcan' have to be used.
 *
 * Project-status: reached beta state. No bugs known, but at the moment 
 * still basic functionality. \n
 * \n
 * Contact: joerg.diederich@graffiti.net
 */

#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "Kernel.h"
#include "S_Timer.h" 
#include "irq_drv.h"
using namespace irq;
#include "mcu_drv_can128.h"
using namespace mcu;

/*
  somehow (GCC Bugzilla Bug 29141), global static objects aren't possible :(
  so just declarations here .. 
*/
#include "servers.h"

/*!
 * \brief Entry from another application.
 *
 * In order to jump into Shumway from another application, this fixed entry
 * point is defined. It is located at a fixed and therefore known address.
 *
 * \note In contrast to the reset entry, a jump from an application
 * did not activate a restart timer.
 *
 * \sa main
 */
void do_bootstrap() __attribute__ ((section (".loader")));

/*!
 * \brief Pure-virtual workaround.
 *
 * The avr-libc does not support a default implementation for handling 
 * possible pure-virtual calls. This is a short and empty workaround for this.
 */
extern "C" {
  void __cxa_pure_virtual()
  {
    // put error handling here
  }
}

/*!
 * \brief ISR for unimplemented interrupts.
 *
 * In order to prevent enabled irq's left by a calling appliction, every
 * irq not implemented by this has to be ignored.
 *
 * \note This does NOT make sure all irq flags are disabled, if the 
 * bootloader calls the application again. So it is still possible, unknown
 * irq's will be activated again and maybe crash your application.
 */
EMPTY_INTERRUPT(__vector_default);

/*!
 * \brief Initialises bootloader.
 *
 * Preparation of bootloader start. Requests mcu to move irq vector table 
 * into bootloader section and instantiates all server.
 */
void
_init_bootstrap(){
  // move irq vector table to bootloader area
  change_irq_vector_to_bl();
  
  /*! \cond DOXYGEN_IGNORE_SERVER_INCLUDE */
  // .. and definitions here :(
#include "servers.cpp"
  /*! \endcond */
}

/*!
 * \brief Start bootloader.
 *
 * Bootloader start. This means only instantiating a kernel for the servers.
 */
void
_start_bootstrap(){
  // start by instantiating kernel
  Kernel();
}

void
do_bootstrap()
{
  // first step: disable possible active irq's
  cli();
  // compute bootloader entry. done at compile time (it's a define)
  uint16_t __bl_start = (uint16_t) GET_BL_SECTION_START();
  // jump to address
  __asm__ __volatile__				 \
    (                                            \
     "movw r30,%0\n\t"				 \
     "ijmp \n\t"				 \
     :						 \
     : "r" ((uint16_t) __bl_start)		 \
     : 
    );                         
}

/*!
 * \brief Program entry.
 *
 * Program run starts right here. 
 *
 * \note Looks for reset status in order to know, if called from reset or not.
 * In the first case, a restart timer is additionally activated by adding 
 * one more server just for this.
 *
 * \return 0 in any case. This should never happen too, as the Kernel is runing
 * in an endless loop.
 */
int
main(void) 
{
  // first step: disable possible active irq's
  cli();
  // init own stuff
  _init_bootstrap();
  // add restart server, if coming from reset
  if ( get_reset_status() ){
//    static S_Timer _s_timer;

// Patched by Michael Schulze
  DDRA=0;
  PORTA=255;
  if ((PINA&16)) {
	appl_start();
  } else {
	// LED blinks three times
        uint8_t i,o;
        for(o=0;o<5;++o) {
        	DDRB^=(1<<7);
        	for(i=0;i<10;++i)
			 _delay_ms(250);
	}
  }
// Patched end

    clear_reset_status();
  }

  _start_bootstrap();
  return 0;
}

