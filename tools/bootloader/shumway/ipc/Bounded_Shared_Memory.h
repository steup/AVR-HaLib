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

#ifndef __BOUNDED_SHARED_BUFFER_H
#define __BOUNDED_SHARED_BUFFER_H

#include <stdint.h>
#include <stddef.h>

#include "config.h"


/*!
 * Type of shared memory identifiers.
 *
 * \todo Check, if better in class or still global like this.
 */
typedef uint8_t shm_key_t;

/*!
 * \class Bounded_Shared_Memory
 * \brief Shared memory and management.
 *
 * The idea is taken from a POSIX implementation. Instances of this holds 
 * a configurated number of pieces of (configurated) size. On request, 
 * a piece is reserved and given out for usage. In reverse, a piece can 
 * be free-ed so that in can be used by others again. Control commands 
 * are thinkable to manipulate included memory, but at the moment only one
 * is implemented. \n
 * Uses a nested class inside for memory handling.
 *
 * \sa SHM_NUM, SHM_SIZE, CTL_OFFSET
 *
 * \note Usage of default constructor.
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class Bounded_Shared_Memory {

 public:
  /*
   * \brief Control commands.
   *
   * Enumeration of shm control commands.
   */
  enum CTL{
    CTL_OFFSET, /*!< Change offset of memory pointer in handle. */
  };

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(Bounded_Shared_Memory);


  /*!
   * \brief Get memory.
   *
   * Request for a memory piece of minimum the given size.
   *
   * \param[in] __size Requested memory size.
   * \param[out] __id Identifier of given memory.
   *
   * \return False On success. \n
   * True If no memory of that size is available.
   */
  bool get(size_t __size, shm_key_t& __id);

  /*!
   * \brief Get memory with maximal size.
   *
   * Request for a memory piece with maximal size.
   *
   * \param[out] __id Identifier of given memory.
   *
   * \return False On success. \n
   * True If no memory of that size is available.
   */
  bool get_max(shm_key_t& __id){
    return get(SHM_SIZE, __id);
  }

  /*!
   * \brief Get size of memory.
   *
   * Request for the size of a memory piece.
   *
   * \param[in] __id Identifier of memory.
   *
   * \return Size of memory. \n
   * 0 if this memory does not exists.
   */
  size_t get_size(const shm_key_t& __id);

  /*!
   * \brief Attach to memory.
   *
   * In order to gain access to a reserved memory, caller has to attach.
   *
   * \param[in] __id Identifier of memory.
   *
   * \return Pointer to memory on success. \n
   * NULL, if memory identifier is unknown or memory wasn't reserved before.
   *
   * \note No check is performed, who reserved memory before.
   */
  uint8_t* attach(const shm_key_t& __id);

  /*!
   * \brief Free memory.
   *
   * Gives control of memory back.
   *
   * \param[in] __id Identifier of memory.
   *
   * \return False On Success. \n
   * True, if memory identifier does not exists.
   */
  bool free(const shm_key_t& __id);

  /*!
   * \brief Get and attach.
   *
   * Combines get and attach of a memory piece.
   *
   * \param[in] __size Requested memory size.
   * \param[out] __id Identifier of given memory.
   *
   * \return Pointer to memory on success. \n
   * NULL, if no memory of that size is available.
   *
   * \sa get, attach
   */
  uint8_t* get_attached(size_t __size, shm_key_t& __id);

  /*!
   * \brief Control memory piece.
   *
   * Perform control commands on memory piece.
   *
   * \param[in] __id Identifier of given memory.
   * \param[in] __ctl Control command.
   * \param[in] __value Control value.
   *
   * \return False On Success. \n
   * True, if command or identifier does not exists.
   *
   * \sa CTL
   */
  bool ctl(const shm_key_t& __id, CTL __ctl, uint8_t __value);

  /*!
   * Volatile key. Useful for signals without data content.
   */
  static shm_key_t S_volatile_key;
  

 private:
  // using a nested class is cheaper than a struct with additional
  // initalizing
  class Shared_Memory_Handle {
  public:
    Shared_Memory_Handle() : _lock(false), _size(SHM_SIZE), _offset(0){
    }
    
    void lock(){
      _lock = true;
    }
    bool is_locked(){
      return _lock;
    }
    void unlock(){
      _lock = false;
      _offset = 0;
    }
    uint8_t* get_data(){
      return (_data + _offset);
    }
    size_t get_size(){
      return (_size - _offset);
    }
    bool add_offset(uint8_t __value){
      if ( __value < (_size - _offset)){
	_offset += __value;
	return false;
      }
      else{
	return true;
      }
    }

  private:
    bool _lock;
    const size_t _size;
    uint8_t _offset;
    uint8_t _data[SHM_SIZE];
  };

  static bool _get_key(uint8_t __idx, shm_key_t& __key){
    __key =  __idx;
    return ! (__key < _size);
  }
  static bool _get_idx(shm_key_t __key, uint8_t& __idx){
    __idx = __key;
    return ! (__idx < _size);
  }

  static const uint8_t _size = SHM_NUM;
  Shared_Memory_Handle _smhs[SHM_NUM]; 
};

#endif /* __BOUNDED_SHARED_BUFFER_H */
