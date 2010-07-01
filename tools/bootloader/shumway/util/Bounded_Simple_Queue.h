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

#ifndef __BOUNDED_SIMPLE_QUEUE_H
#define __BOUNDED_SIMPLE_QUEUE_H

#include "config.h"

/*!
 * \class BoundedSimpleQueue
 * \brief Template of a queue.
 *
 * A queue of limited size. It forms a ringbuffer, where first and last element
 * spinnng during operations. Operation is simply FIFO. 
 * Elements can be anything, but note that elements are copied and not only 
 * pointered to.
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
template<uint8_t __size, class T>
  class Bounded_Simple_Queue{

 public:

  /*!
   * \brief Default constructor.
   *
   * Default constructor. Initializes member variables.
   */
  Bounded_Simple_Queue(): _front(0), 
    _back(0), 
    _fill(0), 
    _rthreads(0), 
    _wthreads(0){
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(Bounded_Simple_Queue);
  
  /*!
   * \brief Push element.
   *
   * Push an element into the queue. Pays attention for interruptions by others
   * by itself.
   *
   * \param[in] __t Constant reference to the element beeing pushed.
   *
   * \return False on success. \n
   * True If queue is full.
   */
  bool push (const T& __t){
    if ( ! Bounded_Simple_Queue::full() ){
      _wthreads++;
      _elements[_back + _wthreads - 1] = __t;
      _wthreads--;
//      patched by mschulze
//      _back = (_back + 1) % __size;
      ++_back;
      if (_back == __size )
          _back = 0;
//      patched by mschulze
      _fill++;
      return false;
    }
    else{
      return true;
    }
  }

  /*!
   * \brief Pop element.
   *
   * Gets the element at front of the queue.. Pays attention for 
   * interruptions by others by itself.
   *
   * \param[out] __t Reference to fill with front element of queue.
   *
   * \return False on success. \n
   * True If queue is empty.
   */
  bool pop(T& __t){
    if ( ! Bounded_Simple_Queue::empty()){
      _rthreads++;
      __t = _elements[_front + _rthreads - 1];
      _rthreads--;
//      patched by mschulze
//      _front = (_front + 1) % __size;
      ++_front;
      if (_front == __size )
          _front = 0;
//      patched by mschulze
      _fill--;
      return false;
    }
    else{
      return true;
    }
  }

  /*!
   * \brief Queue state empty.
   *
   * Looks, if queue is empty. Respects any interrupted queue readers.
   *
   * \return True, if queue is empty. \n
   * False otherwise.
   */
  bool empty() const{
    return (_fill - _rthreads) == 0;
  }

  /*!
   * \brief Queue state full
   *
   * Looks, if queue is full. Respects any interrupted queue writers.
   *
   * \return True, if queue is full. \n
   * False otherwise.
   */
  bool full() const{
    return (_fill + _wthreads ) == __size;
  }
  

 private:
  uint8_t  _front;
  uint8_t  _back;
  uint8_t _fill;
  uint8_t _rthreads;
  uint8_t _wthreads;
  T _elements[__size];
};

#endif /* __BOUNDED_SIMPLE_QUEUE_H */
