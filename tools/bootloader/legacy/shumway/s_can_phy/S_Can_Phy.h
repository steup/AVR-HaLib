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

/*$Id: S_Can_Phy.h 484 2010-07-01 13:25:33Z mschulze $*/

#ifndef __S_CAN_PHY_H
#define __S_CAN_PHY_H

#include <stdint.h>
#include <string.h>

#include "config.h"
#include "types.h"
#include "Server.h"
#include "Message.h"
#include "can_drv.h"
using namespace can;
#include "board_drv_ktb_can128.h"
using namespace board;
#include "i_header.h"
#include "debug.h"


/*!
 * \def _DEBUG_CAN
 *
 * Enables creation of debug messages, if set.
 */
#define _DEBUG_CAN 0

#if _DEBUG_CAN

/*!
 * \def DEBUG_CAN
 *
 * Sends a debug message with the message-string under name of this server.
 */
#define DEBUG_CAN(__dbg_msg ) DEBUG("CAN: ", __dbg_msg)
#else

/*!
 * \def DEBUG_CAN
 *
 * Defines no debugging.
 */
#define DEBUG_CAN(__dbg_msg )
#endif /* _DEBUG_CAN */

/*!
 * \class S_Can_Phy
 * \brief Server responsible for operations at the CAN interface.
 *
 * Servers of this deal with the CAN port. As always, application dependend
 * work in the receive function.
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class S_Can_Phy : public Server {

 public: 
  /*!
   * \brief Default constructor.
   *
   * Initialises the CAN port. Enables irq-receive of CAN messages.
   */
  S_Can_Phy() : _rx_mob_id(MOb_0), _tx_mob_id(MOb_1){
    _S_can_irq_handler_id = _id;
    // FIXME:: type-safe
    init(BAUDRATE_SETTING_1, BAUDRATE_SETTING_2, BAUDRATE_SETTING_3);
    register_rx(_rx_mob_id,&S_can_irq_receive);
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(S_Can_Phy);

  /*!
   * \brief Reception of messages.
   *
   * Handling of message. \n
   * Several messages will be handled.
   * \li \c SIGNAL_ID_OUT Those are sent out via the CAN port.
   * \li \c SIGNAL_ENABLE_ID_FILTER Enables identifier filter on CAN port.
   * \li \c SIGNAL_DISABLE_ID_FILTER Disables identifier filter on CAN port.
   * \li \c SIGNAL_APPL_START Disables reception of CAN messages by irq.
   *
   * Other messages are transfered back to mediator. Messages send by itself
   * are discarded.
   *
   * \param[in] __msg Reference to received message.
   *
   * \sa SIGNAL_ID_OUT, SIGNAL_ENABLE_ID_FILTER, SIGNAL_DISABLE_ID_FILTER, 
   * SIGNAL_APPL_START
   */
  void receive(Message& __msg);

  /*!
   * \brief Entry for CAN isr.
   *
   * This provides the callback for the used CAN driver to an instance of this.
   * For this, instance has to register with it's id, as done in constructor.
   *
   * \param[in] __cmd Reference to CAN data structure from used CAN driver.
   *
   * \todo Still unsafe. Only one instance may register.
   */
  static void S_can_irq_receive(const can::cmd_t& __cmd); 

  /*!
   * \brief Instance callback from CAN interrupt.
   *
   * Evaluates content of CAN message. Creates a message for other servers
   * and sends it via the mediator.
   *
   * \param[in] __cmd Reference to CAN data structure from used CAN driver.
   */
  void can_irq_receive(const can::cmd_t& __cmd);


 private: 
  void can_receive(can::cmd_t __cmd);

  static uint8_t _S_can_irq_handler_id;
  const MOb _rx_mob_id, _tx_mob_id;

  inline void _reply_message(Message& __msg, SIGNAL __reply_sig){
    __msg.set_signal_id(__reply_sig);
    __msg.set_sender_id(_id);
    __msg.toggle_direction();
    _mediator->send(__msg);
  }

};

#endif /* __S_CAN_PHY_H */
