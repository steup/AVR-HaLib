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

#ifndef __MESSAGE_MEDIATOR_H
#define __MESSAGE_MEDIATOR_H

#include "config.h"
#include "types.h"

class Message_Colleague;
class Message;

/*!
 * \class Message_Mediator
 * \brief Parent class of message mediators.
 *
 * Following the mediator pattern, objects of this distributes messages from
 * registered colleagues.
 *
 * \note Pure virtual class.
 */
class Message_Mediator {

 public:
  /*! 
   * \brief Send function.
   *
   * Interface for colleagues to the mediator. 
   * 
   * \param[in] __msg Reference to message to distribute. 
   *
   * \note Pure virtual function.
   */
  virtual void send(const Message& __msg ) = 0;


 protected:
  /*!
   * \brief Default constructor.
   *
   * Prevents public calls.
   */
  Message_Mediator() {}
private:

  /*!
   * \brief Copy constructor.
   *
   * Prevent public copy constructor.
   *
   * \param[in] __m Reference to another message mediator.
   *
   * \note Empty.
   */
  Message_Mediator(const Message_Mediator& __m);

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
 protected:

  V_DEFAULT_DESTRUCTOR(Message_Mediator);

  /*!
   * \brief Assignment operator.
   *
   * Prevent public assignment operator.
   *
   * \param[in] __m Reference to another message mediator.
   *
   * \return Reference to called object.
   */
private:
  const Message_Mediator& operator = (const Message_Mediator& __m);

};

#endif /* __MESSAGE_MEDIATOR_H */
