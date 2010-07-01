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

#ifndef __MESSAGE_COLLEAGUE_H
#define __MESSAGE_COLLEAGUE_H

#include <stdint.h>

#include "config.h"
#include "Message_Mediator.h"

class Message;


/*!
 * \class Message_Colleague
 * \brief Parent class of message colleagues.
 *
 * Following the mediator pattern, all objects which want to communicate with 
 * others over a mediator have to inherit from this.
 *
 * \note Pure virtual class.
 */
class Message_Colleague {

 public:
  /*! 
   * \brief Register function for mediator.
   *
   * Here the mediator can register and therefore inform that it would 
   * accept messages from an instance of this.
   *
   * \param[in] __m Pointer to Mediator. 
   */
  void register_mediator(Message_Mediator *__m) { 
    _mediator = __m; 
  }

  /*! 
   * \brief get identifier.
   *
   * Get identifier of instance of this. Used to distinguish between colleages.
   * Has to be unique for this.
   *
   * \return Identifier of instance.
   */
  uint8_t get_id() const{
    return _id;
  }

  /*! 
   * \brief Receive message.
   *
   * Second part of mediator interface. Mediator calls this to request
   * message handling. This is the heart of all colleague objects.
   *
   * \param[in] __msg Reference to message.
   *
   * \note Pure virtual function. 
   */
  virtual void receive(Message& __msg) = 0;



 protected:
  /*!
   * \brief Default constructor.
   *
   * Requests identifier.
   *
   * \param[in] __id Identifier of instance.
   */
  Message_Colleague(uint8_t __id) : _id(__id){ 
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  V_DEFAULT_DESTRUCTOR(Message_Colleague);


  Message_Mediator *_mediator; /*!< Mediator in use. */
  uint8_t _id; /*!< Identifier. */


 private:
  /*!
   * \brief Copy constructor.
   *
   * Prevent public copy constructor.
   *
   * \param[in] __mc Reference to another message colleage.
   *
   * \note Empty.
   */
  Message_Colleague(const Message_Colleague& __mc);

  /*!
   * \brief Assignment operator.
   *
   * Prevent public assignment operator.
   *
   * \param[in] __mc Reference to another message colleage.
   *
   * \return Reference to called object.
   */
  const Message_Colleague& operator = (const Message_Colleague& __mc);

};

#endif /* __MESSAGE_COLLEAGUE_H */
