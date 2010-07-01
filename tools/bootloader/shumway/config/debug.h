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

/*!
 * \file debug.h
 *\brief Debug settings.
 *
 * Error settings and debug macros. Debug is enabled at the servers in order
 * to enable individual debugging. Error presentation is enabled right here.
 * Debug is done by sending signals to a debug server. If no one is exisiting,
 * the debug message will return to sender after passing all other servers.
 */

/*$Id$*/

#ifndef __DEBUG_H
#define __DEBUG_H

#include <string.h>

/*!
 * \def _ERROR
 * 
 * Enables error debugging by setting to 1.
 */
#define _ERROR 0

/*!
 * \def DEBUG(name, message)
 *
 * Sends a debug message with the name and the message-string.
 */
#define DEBUG(__name, __dbg_msg)					\
  shm_key_t __k;							\
  size_t __dbg_len = strlen(__dbg_msg);					\
  size_t __name_len = strlen(__name);					\
  uint8_t *__m = __shm.get_attached(__dbg_len + __name_len + 1,__k);	\
  if( __m != NULL){							\
    memcpy(__m,(uint8_t*)__name, __name_len);				\
    __m+=__name_len;							\
    memcpy(__m,(uint8_t*)__dbg_msg, __dbg_len + 1);			\
    _mediator->send(Message(SIGNAL_DEBUG, _S_key_to_msg(__k),_id)); \
  }									

#if _ERROR

/*!
 * \def ERROR(message)
 *
 * Sends a debug message with the message-string.
 */
#define ERROR(__dbg_msg )						\
  shm_key_t __k;							\
  size_t __len = strlen(__dbg_msg);					\
  uint8_t *__m = __shm.get_attached(__len + 1,__k);			\
  if( __m != NULL){							\
    memcpy(__m,(uint8_t*)__dbg_msg, __len + 1);				\
    _mediator->send(Message(SIGNAL_DEBUG, _S_key_to_msg(__k),_id)); \
  }									
#else

/*!
 * \def ERROR
 *
 * Defines no error debugging.
 */
#define ERROR(__dbg_msg)
#endif /* _ERROR */

#endif /* __DEBUG_H */
