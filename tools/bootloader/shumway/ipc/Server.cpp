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

#include "Server.h"
#include "i_header.h"

uint8_t Server::_S_server_num = 0;
Server* Server::_S_servers[MAX_SERVER_NUM];

Bounded_Shared_Memory& 
Server::_S_shm()
{
  static Bounded_Shared_Memory __shm;
  return __shm;
}

void 
Server::S_acknowledge(Bounded_Shared_Memory& shm, 
		      ACTION action, 
		      REPLY reply,
		      SIGNAL signal,
		      uint8_t id,
		      Message_Mediator* m){
  shm_key_t __shm_key;
  uint8_t __ack_size = sizeof(i_hdr_action_t) + sizeof(reply);
  i_hdr_action_t *__ia = (i_hdr_action_t*) shm.get_attached(__ack_size, 
							      __shm_key);
  if ( ! __ia ){
    return;
  }
  __ia->action = action;
  __ia->len = __ack_size;
  *( (uint8_t*)(__ia + 1) ) = reply;
  m->send(Message(signal, 
		  _S_key_to_msg(__shm_key),
		  id));
}


