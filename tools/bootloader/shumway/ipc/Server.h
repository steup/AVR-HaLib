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

/*$Id: Server.h 484 2010-07-01 13:25:33Z mschulze $*/

#ifndef __SERVER_H
#define __SERVER_H

#include <stdint.h>

#include "config.h"
#include "types.h"
#include "Message_Colleague.h"
#include "Message.h"
#include "Bounded_Shared_Memory.h"   


/*!
 * \class Server
 * \brief Parent class of all servers.
 *
 * Simply, this is just a container class for all servers. Usage is 
 * to address servers and to provide a common shared memory.
 *
 */
class Server : public Message_Colleague {

 public:
  /*!
   * \brief Get server.
   * 
   * Get server for the given identifier.
   * 
   * \param[in] num Server identifier.
   *
   * \return Pointer to server, if valid identifier. \n
   * NUll Otherwise.
   */
  static Server* get_server(uint8_t num){
    if ( num < _S_server_num ){
      return _S_servers[num];
    }
    else{
      return NULL;
    }
  }
  /*!
   * \brief Get number of servers.
   * 
   * Get number of servers.
   * 
   * \return Server number.\n
   */
  static uint8_t get_server_num(){
    return _S_server_num;
  }


 protected:
  /*!
   * \brief Default constructor.
   *
   * Ensures, each instance inherits with unique identifier.
   */
  Server() : Message_Colleague(_S_server_num){
    // each server has to register himself
    _S_servers[_S_server_num++] = this; 
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  V_DEFAULT_DESTRUCTOR(Server);

  /*!
   * \brief Message identifier to memory identifier.
   *
   * Converts message content identifier to shared memory identifiers.
   *
   * \param[in] __msg_id Message content identifier.
   *
   * \return Appropriate shared memory identifier.
   */
  static shm_key_t _S_msg_to_key(const message_t& __msg_id){
    // silly, but good for now 
    return (shm_key_t)__msg_id;
  }

  /*!
   * \brief Memory identifier to message identifier. 
   *
   * Converts shared memory identifiers to message content identifier.
   *
   * \param[in] __key Shared memory identifier.
   *
   * \return Appropriate message content identifier.
   */
  static message_t _S_key_to_msg(const shm_key_t& __key){
    // silly, but good for now 
    return (message_t)__key;
  }

  /*!
   * \brief Get shared memory.
   *
   * In order to get around GCC Bugzilla Bug 29141, a static call instantiates
   * the shared memory object.
   *
   * \return Reference to shared memory object.
   */
  static Bounded_Shared_Memory& _S_shm(); 

  static void S_acknowledge(Bounded_Shared_Memory& shm, 
			    ACTION action, 
			    REPLY reply,
			    SIGNAL signal,
			    uint8_t id,
			    Message_Mediator* m);

  static uint8_t _S_server_num;  /*!< Number of servers. */
  static Server* _S_servers[]; /*!< All servers. */


 private:
  /*!
   * \brief Copy constructor.
   *
   * Prevent public copy constructor.
   *
   * \param[in] __s Reference to another server.
   *
   * \note Empty.
   */
  Server(const Server& __s);

  /*!
   * \brief Assignment operator.
   *
   * Prevent public assignment operator.
   *
   * \param[in] __s Reference to another server.
   *
   * \return Reference to called object.
   */
  const Server& operator = (const Server& __s);

};

#endif /* __SERVER_H */
