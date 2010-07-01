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

#include "S_Uart_Phy.h"


void 
S_Uart_Phy::receive(Message& __msg)
{
  static Bounded_Shared_Memory& __shm = Server::_S_shm();
  
  if( __msg.get_signal_id() == SIGNAL_DEBUG){
    shm_key_t __shm_key = _S_msg_to_key(__msg.get_msg_id());
    uint8_t *__mem = __shm.attach(__shm_key);

    transmit(UART0,__mem);
    __shm.free(__shm_key);
  }
  else{
    __msg.raise_visits();
    _mediator->send(__msg); 
  }
}
