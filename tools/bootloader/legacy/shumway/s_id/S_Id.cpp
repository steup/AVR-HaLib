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

/*$Id: S_Id.cpp 484 2010-07-01 13:25:33Z mschulze $*/

#include "config.h"
#include "types.h"
#include "mcu_drv.h"
using namespace mcu;
#include "S_Id.h"


void 
S_Id::receive(Message& __msg)
{
  static Bounded_Shared_Memory& __shm = Server::_S_shm();

  shm_key_t __shm_key = _S_msg_to_key(__msg.get_msg_id());
  uint8_t *__mem = __shm.attach(__shm_key);
  SIGNAL __s = __msg.get_signal_id();

  if ( __msg.get_sender_id() == _id){
    DEBUG_ID("Received own message back!\n");
    if ( __s == SIGNAL_COMM_CLOSE ){
	// acknowledge last action
	S_acknowledge(__shm,
		      _ack_action,
		      REPLY_OK,
		      SIGNAL_ID_OUT,
		      _id,
		      _mediator);
	_ack_action = ACTION_INVALID;
	// prepare for start application
	__msg.set_signal_id(SIGNAL_APPL_START);
	__msg.set_direction(Message::DIRECTION_UNKNOWN);
	__msg.set_sender_id(_id);
	_mediator->send(__msg);
    }
    else if ( __s == SIGNAL_APPL_START ){
      // this message has reached all other servers before.
      appl_start();
      // afterwards of course nothing has to be done
    }
    else if ( _ack_action != ACTION_INVALID ){
      S_acknowledge(__shm, 
		    _ack_action, 
		    REPLY_ERROR, 
		    SIGNAL_ID_OUT, 
		    _id, 
		    _mediator); 
      _ack_action = ACTION_INVALID;
      __shm.free(__shm_key);
    }
    else{
      __shm.free(__shm_key);
    }
  }
  else{
    switch ( __s ){
    case SIGNAL_PHY_IN:
      {
	i_hdr_phy_t *__ip = (i_hdr_phy_t*)__mem;
	/*
	  in order to avoid copy's, assignments and more, some kind of
	  cheating has to be used: next header uses half of the phys header, 
	  in detail the size field
	  size-field isn't correct at all. Don't forget to correct it later!
	*/
	if ( __ip->len - sizeof(__ip->id) < sizeof(i_hdr_action_t)){
	  ERROR("ID: Physical data contain no payload.\n");
	  __shm.free(__shm_key);
	  return;
	}
	i_hdr_action_t *__ia = (i_hdr_action_t*)(((uint8_t*)__ip) + 
						 sizeof(__ip->id));
	/*
	  if an action is waiting to be acknowledged, delay
	*/
	if ( _ack_action != ACTION_INVALID ){
	  _mediator->send(__msg);
	  return;
	}
	switch ( __ia->action ){
	case ACTION_SELECT_OPEN:
	  {
	    if ( ! _open){
	      DEBUG_ID("Received open!\n");
	      /* 
		 enable filtering on lower level. if this fails, this 
		 implementation won't work and will reply an error!
	      */
	      __msg.set_signal_id(SIGNAL_ENABLE_ID_FILTER);
	      __msg.set_sender_id(_id);
	      __msg.toggle_direction();
	      _mediator->send(__msg); 
	      _ack_action = ACTION_SELECT_OPEN;
	      return;
	    }
	    break;
	  }
	case ACTION_SELECT_CLOSE:
	  {
	    if (_open){
	      DEBUG_ID("Received close!\n");
	      /*
		notify upper servers. use existing message, content doesn't
		bother someone
	      */
	      __msg.set_signal_id(SIGNAL_COMM_CLOSE);
	      // in order to get it back, if no one is listening
	      __msg.set_sender_id(_id);
	      __msg.set_direction(Message::DIRECTION_UP);
	      _mediator->send(__msg); 
	      _ack_action = ACTION_SELECT_CLOSE;
	      return;
	    }
	    break;
	  }
	default:
	  {
	    if ( _open ){
	      DEBUG_ID("Forward!\n");
	      __shm.ctl(__shm_key, 
			Bounded_Shared_Memory::CTL_OFFSET, 
			sizeof(__ip->id));
	      __ia->len -= sizeof(__ip->id);
	      __msg.set_signal_id(SIGNAL_ID_IN);
	      __msg.set_sender_id(_id);
	      _mediator->send(__msg);
	      return;
	    }
	    break;
	  }
	}
	/*
	  all else is trash: no select actions or invalid open state
	*/
	__shm.free(__shm_key);
	break;
      }
    case SIGNAL_COMM_CLOSED:
      {
	// acknowledge last action
	S_acknowledge(__shm,
		      _ack_action,
		      REPLY_OK,
		      SIGNAL_ID_OUT,
		      _id,
		      _mediator);
	_ack_action = ACTION_INVALID;
	// prepare for start application
	__msg.set_signal_id(SIGNAL_APPL_START);
	__msg.set_direction(Message::DIRECTION_UNKNOWN);
	__msg.set_sender_id(_id);
	_mediator->send(__msg);
	break;
      }
    case SIGNAL_ID_FILTER_ENABLED:
      {
	DEBUG_ID("Filter enabled!\n");
	_open = true;
	_mediator->send(Message(SIGNAL_DISABLE_APPL_START,
				_S_key_to_msg(__shm_key),
				_id));
	S_acknowledge(__shm, 
		      _ack_action, 
		      REPLY_OK, 
		      SIGNAL_ID_OUT, 
		      _id, 
		      _mediator); 
	_ack_action = ACTION_INVALID;
	break;
      }
    case  SIGNAL_TIMER_OUT:
      {
	DEBUG_ID("Restart in preparation!\n");
	_mediator->send(Message(SIGNAL_APPL_START, 
				_S_key_to_msg(__shm_key),
				_id));
	break;
      }
    default:
      // any other messages do not fit and are not from me.
      // back to queue, maybe someone wants them 
      __msg.raise_visits();
      _mediator->send(__msg);
    }
  }
}




