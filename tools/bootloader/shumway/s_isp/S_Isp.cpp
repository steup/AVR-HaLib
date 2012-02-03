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

/*$Id: S_Isp.cpp 484 2010-07-01 13:25:33Z mschulze $*/

#include <string.h>
#include <avr/io.h>

#include "types.h"
#include "debug.h"
#include "S_Isp.h"


// just a definition to satisfy the compiler
uint8_t S_Isp::_S_isp_irq_handler_id;

void 
S_Isp::S_write_complete()
{
    return ((S_Isp*)_S_servers[_S_isp_irq_handler_id])->write_complete();
}

void 
S_Isp::S_erase_complete()
{
    return ((S_Isp*)_S_servers[_S_isp_irq_handler_id])->erase_complete();
}

void 
S_Isp::write_complete()
{
  static Bounded_Shared_Memory& __shm = Server::_S_shm();

  _isp_addr += _isp_buffer_fill;
  _isp_buffer = NULL;
  _isp_buffer_fill = 0;
  __shm.free(_isp_buffer_key);
  _isp_in_progress = false;
}

void 
S_Isp::erase_complete()
{
  _isp_in_progress = false;
}

void 
S_Isp::receive(Message& __msg)
{
  static Bounded_Shared_Memory& __shm = Server::_S_shm();

  shm_key_t __shm_key = _S_msg_to_key(__msg.get_msg_id());
  uint8_t *__mem = __shm.attach(__shm_key);
  SIGNAL __s = __msg.get_signal_id();

  switch (__s){
  case SIGNAL_ID_IN:
    {
      if ( __shm.get_size(__shm_key) < sizeof(i_hdr_action_t) ){
	ERROR("ISP: Received shm contains no action.\n");
	__shm.free(__shm_key);
	return;
      }
      i_hdr_action_t *__ia = (i_hdr_action_t*)__mem;

      if ( _isp_in_progress ){
	// delay message
	_mediator->send(__msg);
	return;
      }
      if ( ! _isp_buffer ){
	// get buffer for data
	if ( __shm.get_max(_isp_buffer_key) ){
	  __shm.free(__shm_key);
	  S_acknowledge(__shm, 
			__ia->action, 
			REPLY_ERROR, 
			SIGNAL_ISP_OUT, 
			_id, 
			_mediator);
	  return;
	}
	else{
	  _isp_buffer = __shm.attach(_isp_buffer_key);
	  _isp_buffer_size = __shm.get_size(_isp_buffer_key);
	}
      }
      // check for buffer first
      if ( _isp_buffer_fill && __ia->action != ACTION_WRITE ){
	if ( _init_isp() ){
	  __shm.free(__shm_key);
	  S_acknowledge(__shm, 
			__ia->action, 
			REPLY_ERROR, 
			SIGNAL_ISP_OUT, 
			_id, 
			_mediator);
	}
	else{
	  _mediator->send(__msg);
	}
	return;
      }

      uint8_t *__data_ptr = __mem + sizeof(i_hdr_action_t);
      uint8_t __data_len = __ia->len - sizeof(i_hdr_action_t);
      REPLY __reply;
      switch (__ia->action){
      case ACTION_MEM_SELECT:
	{
	  if ( __data_len < sizeof(MEMORY_SELECTION)){
	    ERROR("ISP: Memory selection contains no memory.\n");
	  }	    
	  else{
	    MEMORY_SELECTION __ms = *((MEMORY_SELECTION*)__data_ptr);
	    __reply = REPLY_OK;
	    if ( __ms == MS_MEM_FLASH ){
	      _isp_mem_area = FLASH; 
	    }
	    else if ( __ms == MS_MEM_EEPROM ){
	      _isp_mem_area = EEPROM; 
	    }
	    else{
	      __reply = REPLY_ERROR;
	    }
	    S_acknowledge(__shm, 
			  __ia->action, 
			  __reply, 
			  SIGNAL_ISP_OUT, 
			  _id,  
			  _mediator);
	  }
	  __shm.free(__shm_key);
	  return;
	}
      case ACTION_FULL_ERASE:
	{
	  // assign to erase
	  _isp_in_progress = true;
	  if ( full_erase_async(_isp_mem_area, &S_erase_complete)){
	    _isp_in_progress = false;
	    __reply = REPLY_ERROR;
	  }
	  else{
	    __reply = REPLY_OK;
	  }
	  S_acknowledge(__shm, 
			__ia->action, 
			__reply, 
			SIGNAL_ISP_OUT, 
			_id, 
			_mediator);
	  __shm.free(__shm_key);
	  return;
	}
      case ACTION_ADDR:
	{
	  if ( __data_len < sizeof(_isp_addr)){
	    ERROR("ISP: Address selection contains no address.\n");
	  }	    
	  else{
	    _isp_addr = *((uint32_t*)__data_ptr);
	    S_acknowledge(__shm, 
			  __ia->action, 
			  REPLY_OK, 
			  SIGNAL_ISP_OUT, 
			  _id, 
			  _mediator);
	  }
	  __shm.free(__shm_key);
	  return;
	}
      case ACTION_WRITE:
	{
	  if ( _isp_buffer_fill + __data_len > _isp_buffer_size){
	    if ( !  _init_isp() ){
	      _mediator->send(__msg);
	      return;
	    }
	    else{
	      __reply = REPLY_ERROR;
	    }
	  }
	  else{
	    memcpy(_isp_buffer + _isp_buffer_fill, __data_ptr, __data_len);
	    _isp_buffer_fill += __data_len;
	    __reply = REPLY_OK;
	  }
	  S_acknowledge(__shm, 
			__ia->action, 
			__reply, 
			SIGNAL_ISP_OUT, 
			_id, 
			_mediator);
	  __shm.free(__shm_key);
	  return;
	}
      case ACTION_READ:
	{
	  if ( __data_len < sizeof(_isp_addr)){
	    ERROR("ISP: Address for read contains no address.\n");
	    __shm.free(__shm_key);
	    return;
	  }	    

	  uint32_t __dest_addr = *((uint32_t*)__data_ptr);
	  if ( __dest_addr > _isp_addr ){
	    uint8_t __max_buf_size = _isp_buffer_size - sizeof(i_hdr_action_t);
	    uint8_t __size;
	    uint8_t __diff = __dest_addr - _isp_addr;
	    /*
	      if overflow to zero OR diff is too big. Doing so is cheaper
	      in flash than using a 32bit diff
	    */
	    if ( ! __diff || __diff > __max_buf_size ){
	      __size = __max_buf_size;
	    }
	    else{
	      __size = __diff;
	    }
	    memcpy(_isp_buffer, __ia, sizeof(i_hdr_action_t));
	    ((i_hdr_action_t*)_isp_buffer)->len = __size + 
	      sizeof(i_hdr_action_t);
	    if ( read_buffer(_isp_mem_area, 
			     _isp_addr, 
			     _isp_buffer + sizeof(i_hdr_action_t),
			     __size)){
	      __shm.free(__shm_key);
	      S_acknowledge(__shm, 
			    __ia->action, 
			    REPLY_ERROR, 
			    SIGNAL_ISP_OUT, 
			    _id, 
			    _mediator);
	      return;
	    }
	    _mediator->send(Message(SIGNAL_ISP_OUT, 
				    _S_key_to_msg(_isp_buffer_key),
				    _id,
				    Message::DIRECTION_DOWN));
	    _isp_buffer = NULL;
	    _isp_addr += __size;
 	    _mediator->send(__msg);
	    return;
	  }
 	  else{
	      __shm.free(__shm_key);
	      S_acknowledge(__shm, 
			    __ia->action, 
			    REPLY_OK, 
			    SIGNAL_ISP_OUT, 
			    _id, 
			    _mediator);
	    return;
 	  }
	}
      default:
	ERROR("ISP: Invalid action in signal from ID.\n");
	__shm.free(__shm_key);
	break;
      } // action switch for id_signals
      break;
    }
  case SIGNAL_COMM_CLOSE:
    {
      // finish started work 
      if ( _isp_buffer_fill > 0 ){
	if ( ! _isp_in_progress ){
	  _init_isp();
	}
      }
      else{
	__msg.set_signal_id(SIGNAL_COMM_CLOSED);
	__msg.set_sender_id(_id);
	__msg.toggle_direction();
      }
      _mediator->send(__msg);
      return;
    }
  default:
    {
      // not exactly at the right position, but anyway, it works
      if ( __msg.get_sender_id() == _id){
	__shm.free(__shm_key);
      }
      else{
	// any other messages do not fit and are not from me.
	// back to queue, maybe someone wants them 
	__msg.raise_visits();
	_mediator->send(__msg);
      } 
      break;
    }
  }// signal switch
}





