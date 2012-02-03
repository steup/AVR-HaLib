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

/*$Id: Bounded_Shared_Memory.cpp 484 2010-07-01 13:25:33Z mschulze $*/

#include "Bounded_Shared_Memory.h"


shm_key_t Bounded_Shared_Memory::S_volatile_key = SHM_SIZE;

bool
Bounded_Shared_Memory::get(size_t __size, shm_key_t& __id)
{
  uint8_t __idx = _size;

  while (__idx-- && 
	 (_smhs[__idx].get_size() < __size || _smhs[__idx].is_locked())
	 );
  if (__idx == 0xFF){
    return true;
  }
  else{
    _smhs[__idx].lock();
    return _get_key(__idx, __id);
  }
}

size_t
Bounded_Shared_Memory::get_size(const shm_key_t& __id)
{
  uint8_t __idx;
  if (  _get_idx(__id, __idx )){
    return 0;
  }
  else{
    return _smhs[__idx].get_size();
  }
}

uint8_t*
Bounded_Shared_Memory::attach(const shm_key_t& __id)
{
  uint8_t __idx;

  if ( _get_idx(__id, __idx)){
    return NULL;
  }
  if ( _smhs[__idx].is_locked()){
    return _smhs[__idx].get_data();
  }
  else{
    return NULL;
  }
}

bool
Bounded_Shared_Memory::free(const shm_key_t& __id)
{
  uint8_t __idx;
  if ( _get_idx(__id, __idx)){
    return true;
  }
  _smhs[__idx].unlock();
  return false;
}

uint8_t* 
Bounded_Shared_Memory::get_attached(size_t __size, shm_key_t& __id)
{
  if (get(__size,__id)){
    return NULL;
  }
  return attach(__id);
}

bool 
Bounded_Shared_Memory::ctl(const shm_key_t& __id, CTL __ctl, uint8_t __value)
{
  if ( __ctl == CTL_OFFSET ){
    uint8_t __idx;
    if ( ! _get_idx(__id, __idx)){
      return _smhs[__idx].add_offset(__value);
    }
    else{
      return true;
    }
  }
  else{
    return true;
  }
}

