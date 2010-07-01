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

#ifndef __S_UART_PHY_H
#define __S_UART_PHY_H

#include <stdint.h>

#include "config.h"
#include "types.h"
#include "Server.h"
#include "Message.h"
#include "uart.h"
using namespace uart;


/*!
 * \class S_Uart_Phy
 * \brief Server responsible for operations at the UART interface.
 *
 * This server deals with the UART port. As with all servers respectively
 * colleagues, the application-dependend work is done in the receive function. 
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class S_Uart_Phy : public Server {

  public: 
  /*!
   * \brief Default constructor.
   *
   * Initialises an UART port.
   */
  S_Uart_Phy(){ 
    init(UART0);
    set_baudrate(UART0, BAUD_RATE_19200);
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(S_Uart_Phy); 

  /*!
   * \brief Reception of messages.
   *
   * Handling of message. \n
   * If message signalizes a debug message, the content of the message is 
   * transmitted via the initialised UART port. Else, the message is transfered
   * back to mediator.
   *
   * \param[in] __msg Reference to received message.
   *
   * \sa SIGNAL_DEBUG
   */
  void receive(Message& __msg);

};

#endif /* __S_UART_PHY_H */
