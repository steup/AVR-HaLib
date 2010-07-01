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

#ifndef __S_ID_H
#define __S_ID_H

#include <stdint.h>
#include <string.h>

#include "Server.h"
#include "config.h"
#include "debug.h"
#include "protocol.h"
#include "i_header.h"
#include "debug.h"


/*!
 * \def _DEBUG_ID
 *
 * Enables creation of debug messages, if set.
 */
#define _DEBUG_ID 0

#if _DEBUG_ID

/*!
 * \def DEBUG_ID
 *
 * Sends a debug message with the message-string under name of this server.
 */
#define DEBUG_ID(__dbg_msg) DEBUG("ID: ", __dbg_msg)
#else

/*!
 * \def DEBUG_ID
 *
 * Defines no debugging.
 */
#define DEBUG_ID(__dbg_msg)
#endif /* _DEBUG_ID */


/*!
 * \class S_Id
 * \brief Server responsible for various operations.
 *
 * From all servers till now, an instance of this is something special. It
 * represents a connection between hardware-related servers and upper servers.
 * It contains the logic behind everything. 
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class S_Id : public Server {

 public:
  /*!
   * \brief Default constructor.
   *
   * Initialises status.
   */
  S_Id() : 
    _open(false), 
    _ack_action(ACTION_INVALID){
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(S_Id);

  /*!
   * \brief Reception of messages.
   *
   * Handling of message. \n
   * Several messages will be handled.
   * \li \c SIGNAL_PHY_IN From physical interface, these messages have to be 
   * evaluated in more detail, according to the included protocol action.
   * \li \c SIGNAL_COMM_CLOSED Quit work.
   * \li \c SIGNAL_ISP_OUT Signal from above will be relayed.
   * \li \c SIGNAL_ID_FILTER_ENABLED Ready for more operations.
   * \li \c SIGNAL_ID_FILTER_DISABLED Operations finished.
   *
   * Messages sent by itself will be treated as well. This is for the case
   * no one's listening.
   * Other messages are transfered back to mediator. 
   *
   * \param[in] __msg Reference to received message.
   *
   * \sa SIGNAL_PHY_IN, SIGNAL_COMM_CLOSED, SIGNAL_ISP_OUT, 
   * SIGNAL_ID_FILTER_ENABLED, SIGNAL_ID_FILTER_DISABLED, SIGNAL_COMM_CLOSE
   * 
   */
  void receive(Message& __msg);


 private:
  bool _open;
  ACTION _ack_action;

};

#endif /* __S_ID_H */
