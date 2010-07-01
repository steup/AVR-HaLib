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

#ifndef __S_TIMER_H
#define __S_TIMER_H

#include <stdint.h>
#include <string.h>

#include "Server.h"
#include "config.h"
#include "debug.h"
#include "protocol.h"
#include "i_header.h"
#include "debug.h"
#include "board_drv_ktb_can128.h"
using namespace board;


/*!
 * \def _DEBUG_TIMER
 *
 * Enables creation of debug messages, if set.
 */
#define _DEBUG_TIMER 0

#if _DEBUG_TIMER

/*!
 * \def DEBUG_TIMER
 *
 * Sends a debug message with the message-string under name of this server.
 */
#define DEBUG_TIMER(__dbg_msg) DEBUG("TIMER: ", __dbg_msg)
#else

/*!
 * \def DEBUG_TIMER
 *
 * Defines no debugging.
 */
#define DEBUG_TIMER(__dbg_msg )
#endif /* _DEBUG_TIMER */


/*!
 * \class S_Timer
 * \brief Server responsible for restart operations.
 *
 * If instantiated, a server of this has only one purpose: To request a 
 * restart of the  microcontroller after a configured time. Of course it is 
 * listening for any messages telling him to stop his work.
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class S_Timer : public Server {

 public:
  /*!
   * \brief Default constructor.
   *
   * Initialises status. Enables timer-irq for it's operation.
   */
  S_Timer() : 
    _cond_cnt(START_CONDITION_TIME){
    _S_condition_handler_id = _id;
    enable_timed_condition(&S_condition, 1);
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(S_Timer);


  void receive(Message& __msg);

  /*!
   * \brief Entry for timer isr.
   *
   * This provides the callback for the used timer to an instance of this.
   * For this, instance has to register with it's id, as done in constructor.
   */
  static void S_condition(); 

  /*!
   * \brief Instance callback from timer interrupt.
   *
   * Evaluates, if restart condition is reached. If so, all servers have to
   * be noticed for that. So just a message is send around.
   *
   * \note It would be wrong to restart from here, as done in a former version.
   * Because there might be resources (irq, isp, ..) still in use, other 
   * servers have to be informed before to enable appropriate release actions.
   */
  void condition();


 private:
  uint8_t _cond_cnt;
  
  static uint8_t _S_condition_handler_id;
};

#endif /* __S_TIMER_H */
