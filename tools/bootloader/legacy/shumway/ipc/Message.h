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

/*$Id: Message.h 484 2010-07-01 13:25:33Z mschulze $*/

#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "config.h"
#include "types.h"


/*!
 * \class Message
 * \brief Handling object between servers.
 *
 * Communication between servers is running with instances of this. As the 
 * servers architecture can be thought as a stack, movement informations
 * have to be given.
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class Message {

 public:

  /*!
   * \brief Intended move direction.
   *
   * Enumration of directions a message object should take.
   */
  enum DIRECTION{
    DIRECTION_UP, /*!< Direction upwards. */
    DIRECTION_DOWN, /*!< Direction downwards. */
    DIRECTION_UNKNOWN, /*!< Direction unknown.. */
  };

  /*!
   * \brief Default contructor.
   *
   * Nearly unsed, but necessary for instantiating a first object. In order to
   * signalize it is not initialised, it's signal id is invalid.
   *
   * \sa SIGNAL
   */
  Message() : _sig_id(SIGNAL_INVALID), _dir(DIRECTION_UNKNOWN), _visits(0){
  }

  /*!
   * \brief Custom contructor.
   *
   * Completly instantiates an object filled with all necessary informations.
   *
   * \param[in] __sig_id Signal identifier.
   * \param[in] __msg_id Content identifier carried.
   * \param[in] __sender_id Identifier of sender.
   * \param[in] __dir Direction message should move.
   *
   * \sa SIGNAL, DIRECTION
   */
  Message(SIGNAL __sig_id, 
	  message_t __msg_id, 
	  uint8_t __sender_id,
	  DIRECTION __dir = DIRECTION_UNKNOWN) : 
    _sig_id(__sig_id), 
    _msg_id(__msg_id), 
    _sender_id(__sender_id),
    _dir(__dir),
    _visits(0){
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(Message);

  /*!
   * \brief Set signal identifier.
   *
   * Sets signal identifier.
   * 
   * \param[in] __sig_id New signal identifier.
   */
  void set_signal_id(SIGNAL __sig_id){
    _sig_id = __sig_id;
  }

  /*!
   * \brief Get signal identifier.
   *
   * Get signal identifier.
   * 
   * \return Signal identifier.
   */
  SIGNAL get_signal_id() const{
    return _sig_id;
  }

  /*!
   * \brief Set message identifier.
   *
   * Sets identifier of content carried.
   * 
   * \param[in] __msg_id New message identifier.
   */
  void set_msg_id(message_t __msg_id) {
    _msg_id = __msg_id;
  }

  /*!
   * \brief Get content identifier.
   *
   * Get content identifier.
   * 
   * \return Content identifier.
   */
  message_t get_msg_id() const{
    return _msg_id;
  }

  /*!
   * \brief Set sender identifier.
   *
   * Sets sender identifier.
   * 
   * \param[in] __sender_id New sender id
   */
  void set_sender_id(uint8_t __sender_id){
    _sender_id = __sender_id;
    _visits = 0;
  }

  /*!
   * \brief Get sender identifier. 
   *
   * Get sender identifier.
   * 
   * \return Identifier of sender.
   */
  uint8_t get_sender_id() const{
    return _sender_id;
  }

  /*!
   * \brief Set direction.
   *
   * Set direction where object should move to.
   * 
   * \param[in] __dir New direction.
   */
  void set_direction(DIRECTION __dir){
    _dir = __dir;
  }

  /*!
   * \brief Toggle direction.
   *
   * Toggle direction of object moving. Helpful for simple reply messages.
   */
  void toggle_direction();

  /*!
   * \brief Get direction. 
   *
   * Get direction object should move to.
   * 
   * \return Direction.
   */
  DIRECTION get_direction() const{
    return _dir;
  }

  /*!
   * \brief Get visits. 
   *
   * Get number of visits an object has already made at servers.
   * 
   * \return Number of visits.
   */
  uint8_t get_visits(){
    return _visits;
  }

  /*!
   * \brief Raise visits. 
   *
   * Increase number of visits an object has already made.
   */
  void raise_visits(){
    _visits++;
  }


 protected:
  SIGNAL _sig_id; /*!< Signal identifier. */
  message_t _msg_id; /*!< Content identifier. */
  uint8_t _sender_id; /*!< Sender identifier. */
  DIRECTION _dir; /*!< Intended move direction. */
  uint8_t _visits; /*!< Number of visits made. */
  
};

#endif /* __MESSAGE_H */
