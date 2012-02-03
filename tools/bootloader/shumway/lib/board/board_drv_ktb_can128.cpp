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

/*$Id: board_drv_ktb_can128.cpp 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file board_drv_ktb_can128.cpp
 * \brief Board specific functions.
 * 
 * Definitions of board specific driver of KTB board with at90can128 mcu.
 */

#include "board_drv_ktb_can128.h"

#include "mcu_drv.h"
using namespace mcu;
#include "timer1_drv.h"
using namespace timer1;


namespace board
{
  bool 
  enable_timed_condition(void (*cb)(), uint8_t res){
    bool __rc;
    // set input timer1. 62500 is the value for one second at prescale 256
// patched by mschulze
//    set_input_capture_val(62500 / res);
    set_input_capture_val(62500);
    enable(CS_256, MOOP_12);
    __rc = register_input_capture(cb);
    enable_input_capture_irq();
    return __rc;
  }

  void 
  disable_timed_condition(){
    disable_input_capture_irq();
    unregister_input_capture();
  }

} // namespace board
