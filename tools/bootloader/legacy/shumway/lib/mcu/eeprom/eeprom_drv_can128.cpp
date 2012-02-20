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

/*$Id: eeprom_drv_can128.cpp 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file eeprom_drv_can128.cpp
 * \brief MCU specific eeprom driver definitions.
 * 
 * Eeprom driver for the AT90CAN128 mcu.
 */

#include <stddef.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "eeprom_drv_can128.h"


namespace eeprom
{
  void (*_cb)(void) = NULL;

  // all-purpose: addr, buffer, counter variables
  uint16_t _addr;
  uint16_t _dest_addr;
  uint8_t* _buffer;

  // state variables
  bool _erase;

  bool read_buffer(const uint16_t& addr, 
		   uint8_t* buffer, 
		   const uint16_t& buffer_len)
  {
    for (uint16_t __i = 0; __i < buffer_len; __i++){
      buffer[__i] = read_byte(addr + __i);
    }
    return false;
  }

  bool 
  full_erase_async(void (*cb)(void)){
    if ( ! _cb ){
      _erase = true;
      // set start address
      _addr = 0;
      // set number of bytes to erase
      _dest_addr = (uint16_t)E2END;
      return init_async(cb);
    }
    return true;
  }

  bool
  write_buffer_async(const uint16_t& addr, 
		     uint8_t* buffer, 
		     const uint16_t& buffer_len,
		     void (*cb)(void))
  {
    if ( ! _cb ){
      _erase = false;
      // set start address
      _addr = addr;
      _dest_addr = addr + buffer_len - 1;
      _buffer = buffer;
      return init_async(cb);
    }
    return true;
  }

  bool
  init_async(void (*cb)(void)){
    // register callback
    _cb = cb;
    // enable eeprom irq
    EECR = (1<<EERIE);
    // enable global irq
    sei();
    return false;
  }

  ISR(EE_READY_vect)
  {
    uint8_t __data=0;
    while ( _addr <= _dest_addr ){
      if ( _erase ){
	__data = BLANK_BYTE;
      }
      else{
	__data = *_buffer++;
      }
      if ( read_byte(_addr) == __data){
	_addr++;
      }
      else{
	break;
      }
    }
    if ( _addr <= _dest_addr ){
      // see proceeding at page 22
      // wait for spm to become ready. looks strange here, but
      // could take max 4.5ms (page 330)
      while( bit_is_set(SPMCSR,SPMEN) );
      EEAR = _addr++; 
      EEDR = __data;
      EECR |= (1<<EEMWE); // writing logical 0 for EEWE ignored, as it is 0
      EECR |= (1<<EEWE);
    }
    else{
      // notify listener, if registered
      if ( _cb ){
	(*_cb)();
	// hang out listener
	_cb = NULL;
      }
      // disable eeprom irq
      EECR &= ~(1<<EERIE);
    }
  }

} // namespace eeprom
