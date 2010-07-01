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

/*$Id$*/

/*!
 * \file timer1_drv_can128.cpp
 * \brief MCU specific driver for timer 1.
 * 
 * Definitions of timer driver for the AT90CAN128 mcu timer 1.
 */

#include <stdint.h>
#include <avr/interrupt.h>

#include "timer1_drv_can128.h"


namespace timer1
{
  /*!
   * Callback for interrupt service routine..
   */
  void (*_cb_timer1_capt_vect)() = NULL;

  bool register_input_capture(void (*cb)())
  {
    if ( _cb_timer1_capt_vect == NULL ){
      _cb_timer1_capt_vect = cb;
      return false;
    }
    return true;
  }
  
  void unregister_input_capture()
  {
    _cb_timer1_capt_vect = NULL;
  }



  void enable(CLOCK_SELECT cs, MOOP mode){
    disable_output_compare_all();
    set_prescaler(cs);
    set_mode_of_operation(mode);
  }

  /*!
   * \brief ISR for timer 1.
   *
   * Interrupt service routine of Timer/Counter1 Capture Event interrupt.
   */
  ISR(TIMER1_CAPT_vect)
  {
    uint8_t __cp_save = CANPAGE;
    (*_cb_timer1_capt_vect)();
    CANPAGE = __cp_save;
  }

} // namespace timer1
