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

#include "Kernel.h"
#include "types.h"
#include "Message_Colleague.h" 
#include "Server.h" 


Kernel::Kernel() : Message_Mediator()
{
  _init();
  _run();
}

void
Kernel::_init(){
  uint8_t __i = 0;
  Message_Colleague* __mc;
  while ( (__mc = (Message_Colleague*)Server::get_server(__i++)) ){
    __mc->register_mediator(this);
  }
}

void
Kernel::_run()
{
  Message __msg;
  Message_Colleague* __mc;

  while(1){
    while ( _mq.empty() );
    while ( ! _mq.pop(__msg) ){
      uint8_t __sender_id = __msg.get_sender_id();
      uint8_t __visits = __msg.get_visits() + 1;
      uint8_t __target_id = __sender_id;
      Message::DIRECTION __dir = __msg.get_direction();
      
      if ( __dir == Message::DIRECTION_UP ){
	__target_id += __visits;
      }
      else if ( __dir == Message::DIRECTION_DOWN ){
	__target_id -= __visits;
      }
      else{
	// direction unknown
	__target_id = __msg.get_visits();
	// skip original sender
 	if ( __target_id == __sender_id ) {
	  __target_id++;
 	}
      }
      __mc = (Message_Colleague*)Server::get_server(__target_id);
      if ( ! __mc ){
	__mc = (Message_Colleague*)Server::get_server(__sender_id);
      }
      __mc->receive(__msg);
      continue;
    }
  } // endless loop
}

void 
Kernel::send(const Message& __msg)
{
  _mq.push(__msg);
}
