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

#include <stddef.h>

#include "S_Can_Phy.h"
#include "i_header.h"
#include "debug.h"


// just a definition to satisfy the compiler
uint8_t S_Can_Phy::_S_can_irq_handler_id;

void 
S_Can_Phy::S_can_irq_receive(const can::cmd_t& __cmd)
{
  return ((S_Can_Phy*)_S_servers[_S_can_irq_handler_id])->
    can_irq_receive(__cmd);
}

void 
S_Can_Phy::can_irq_receive(const can::cmd_t& __cmd)
{
  static Bounded_Shared_Memory& __shm = Server::_S_shm();

  DEBUG_CAN("Received can message with data in it.\n");
  
  shm_key_t __shm_key;
  uint8_t* __mem = __shm.get_attached(sizeof(i_hdr_phy_t) + __cmd.dlc, 
				      __shm_key);
  if ( __mem == NULL ){
    ERROR("Unable to get buffer for recved can msg.\n");
    return;
  }
  i_hdr_phy_t *__ih = (i_hdr_phy_t*)__mem;
  __ih->id = __cmd.id.ext;
  __ih->len = sizeof(i_hdr_phy_t) + __cmd.dlc;
  memcpy(__mem + sizeof(i_hdr_phy_t), __cmd.data, __cmd.dlc);
  _mediator->send(Message(SIGNAL_PHY_IN, 
			  _S_key_to_msg(__shm_key),
			  _id,
			  Message::DIRECTION_UP));
  return;
}

void 
S_Can_Phy::receive(Message& __msg)
{
  static Bounded_Shared_Memory& __shm = Server::_S_shm();
  shm_key_t __shm_key = _S_msg_to_key(__msg.get_msg_id());
  SIGNAL __s = __msg.get_signal_id();
  uint8_t *__mem = __shm.attach(__shm_key);

  if ( __msg.get_sender_id() == _id){
    ERROR("CAN: Received own message back!\n");
    __shm.free(__shm_key);
  }
  else{
    // prepare can cmd 
    cmd_t __cmd;
    __cmd.ide = true;

    if ( __s == SIGNAL_ID_OUT || __s == SIGNAL_ISP_OUT ){
      DEBUG_CAN("Received outgoing message!\n");
      if ( __mem ){
	struct i_hdr_action_t *__ia = (i_hdr_action_t*)__mem;
	uint8_t __len = __ia->len - sizeof(i_hdr_action_t);
	uint8_t __i = 0;
	uint8_t __dlc;

	// turn into can message, transmitted extended by MOb 1(decision)
	__cmd.mob_id = _tx_mob_id;
	__cmd.id.ext = get_own_id();
	__cmd.data[0] = __ia->action;

 	__mem += sizeof(i_hdr_action_t);
	do {
	  if ( (__len - __i) < (can::DATA_LEN - 1) ){
	    __dlc = __len - __i;
	  }
	  else{
	    __dlc = can::DATA_LEN - 1;
	  }
 	  memcpy(__cmd.data + 1, __mem, __dlc);
	  __cmd.dlc = __dlc + 1;
	  __mem += __dlc;
	  __i += __dlc;
 	  send(__cmd);
	} 
	while ( __i < __len );
      }
      __shm.free(__shm_key);
      return;
    }
    else if ( __s == SIGNAL_ENABLE_ID_FILTER ){
      if ( __mem ){
	DEBUG_CAN("Enabling filter!\n");
	struct i_hdr_phy_t *__ih = (i_hdr_phy_t*)__mem;
	__cmd.mob_id = _rx_mob_id;
	__cmd.id.ext = __ih->id;
 	enable_msk(__cmd);	
      	_reply_message(__msg, SIGNAL_ID_FILTER_ENABLED);
      }
      else{
	__shm.free(__shm_key);
      }
      return;
    }
    else if ( __s == SIGNAL_APPL_START ){
      __cmd.mob_id = _rx_mob_id;
      disable_msk(__cmd);
      unregister_rx(_rx_mob_id);
    }
    __msg.raise_visits();
    _mediator->send(__msg); 
    return;
  }
}






