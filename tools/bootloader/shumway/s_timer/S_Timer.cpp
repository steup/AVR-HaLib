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

#include "config.h"
#include "types.h"
#include "S_Timer.h"


// just a definition to satisfy the compiler
uint8_t S_Timer::_S_condition_handler_id;

void 
S_Timer::S_condition()
{
  return ((S_Timer*)_S_servers[_S_condition_handler_id])->
    condition();
}

void 
S_Timer::condition()
{
  if ( ! --_cond_cnt ){
    disable_timed_condition();
    _mediator->send(Message(SIGNAL_TIMER_OUT, 
 			    Bounded_Shared_Memory::S_volatile_key,
 			    _id));
  }
}

void 
S_Timer::receive(Message& __msg)
{
  switch ( __msg.get_signal_id() ){
  case SIGNAL_DISABLE_APPL_START:
    {
      disable_timed_condition();
      Server::_S_shm().free(_S_msg_to_key(__msg.get_msg_id()));
    }
  default:
    // any other messages do not fit and are not from me.
    // back to queue, maybe someone wants them 
    __msg.raise_visits();
    _mediator->send(__msg);
    break;
  }
}
