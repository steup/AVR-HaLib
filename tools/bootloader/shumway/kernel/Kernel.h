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

#ifndef __KERNEL_H
#define __KERNEL_H

#include <stdint.h>

#include "config.h"
#include "Message_Mediator.h" 
#include "Message.h" 
#include "Bounded_Simple_Queue.h" 


/*!
 * \class Kernel
 * \brief Kernel of application.
 *
 * Basically this is just a message distributor for servers, which are 
 * instantiated somewhere else. For this, a queue is used, into which
 * servers put their messages into. 
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class Kernel : public Message_Mediator {

 public:
  
  /*!
   * \brief Default constructor.
   *
   * Default constructor. Starts processing and never returns, as the kernel 
   * should'nt quit it's work.
   */
  Kernel();

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(Kernel); 

  /*!
   * \brief Send interface.
   *
   * Interface for servers to give their messages to the kernel.
   *
   * \param[in] msg The message given.
   */
  void send(const Message& msg); 


 private:
  void _init();
  void _run(); 

  Bounded_Simple_Queue<MAX_SIGNAL_NUM, Message> _mq; 
};

#endif /* __KERNEL_H */
