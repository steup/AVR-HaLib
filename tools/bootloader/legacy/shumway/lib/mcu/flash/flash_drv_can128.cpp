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

/*$Id: flash_drv_can128.cpp 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file flash_drv_can128.cpp
 * \brief MCU specific flash driver definitions.
 * 
 * Flash driver for the AT90CAN128 mcu.
 */

#include <stddef.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>

#include "flash_drv_can128.h"


namespace flash
{
  // private prototypes
  void _irq_page_load_handler();
  void _irq_page_write_handler();
  void _irq_page_erase_handler();
  void _irq_exit_handler();
  
  void (*_cb)(void) = NULL;
  // enable switch of irq handlers
  void (*_irq_handler)(void) = NULL;

  // status remember variables while writing
  uint16_t _tmp_buffer_fill;
  uint32_t _tmp_addr;

  // all-purpose: addr, buffer, number of something
  uint32_t _dest_addr;
  uint8_t* _buffer;
  uint16_t _num;
  

  bool read_buffer(const uint32_t& addr, 
		   uint8_t* buffer, 
		   const uint16_t& buffer_len)
  {
    for (uint16_t __i = 0; __i < buffer_len; __i++){
      buffer[__i] = read_byte(addr + __i);
    }
    return false;
  }

  bool
  page_blank_check(const uint32_t& addr)
  {
    uint32_t __aligned_addr = addr &  (~((uint32_t)SPM_PAGESIZE - 1));

    for ( uint16_t __i = SPM_PAGESIZE; __i ; __i-- ){
      if ( read_byte(__aligned_addr++) != (uint8_t)BLANK_BYTE ){
	return true;
      }
    }
    return false;
  }

  bool 
  full_erase_async(void (*cb)(void)){
    if ( ! _cb ){
      // register callback
      _cb = cb;
      _irq_handler = &_irq_page_erase_handler;
      // set start adress
      _dest_addr = 0;
      // set number of steps. this is word sized, not byte
      _num = ((FLASHEND + 1)/ 2) - GET_APPL_SECTION_SIZE_W();
      // enable spm irq
      SPMCSR = (1<<SPMIE);
      // enables global irq
      sei();
      return false;
    }
    return true;
  }

  bool
  write_buffer_async(const uint32_t& addr, 
		     uint8_t* buffer, 
		     const uint16_t& buffer_len,
		     void (*cb)(void))
  {
    if ( ! _cb ){
      _dest_addr = addr;
      _buffer = buffer;
      _num = buffer_len;
      _cb = cb;
      _irq_handler = &_irq_page_load_handler;
      // have to provide an aligned adress in temporary
      _tmp_addr = addr &  (~((uint32_t)SPM_PAGESIZE - 1));
      _tmp_buffer_fill = 0;
      // enable irq
      SPMCSR = (uint8_t)_BV(SPMIE);
      // enables global irq
      sei();
      return false;
    }
    return true;
  }

  void
  _irq_page_erase_handler()
  {
    if ( _num ){
      boot_page_erase_irq(_dest_addr);
      // next. can be done afterwards cause irq doesn't get lost
      _dest_addr += SPM_PAGESIZE;
      _num -= (SPM_PAGESIZE/2);
    }
    else{
      // we are done
      _irq_handler = &_irq_exit_handler;
      boot_rww_enable_irq();
    }
  }

  // loads temporary page buffer. only one word each, after this waits for
  // an irq. this signalises ready for more.
  void
  _irq_page_load_handler(){
    if ( _tmp_buffer_fill == (SPM_PAGESIZE) ){
      // last page load results in full temp buffer
      if ( page_blank_check(_dest_addr) ){
	boot_page_erase_irq(_dest_addr); 
 	_irq_handler = _irq_page_write_handler;	
      }
      else{
	// write page, as page is blank for sure
	  boot_page_write_irq(_dest_addr); 
	  _tmp_buffer_fill = 0;
 	  _dest_addr = _tmp_addr;
       }
    }
    else if ( (! _num) && (! _tmp_buffer_fill) ){
      // nothing to do and buffer was written/is empty -> finished
      _irq_handler = &_irq_exit_handler;
      // re-enable rww before
      boot_rww_enable_irq();
    }
    else{
      uint16_t __data = 0;
      uint8_t __endian_shift = 0;
      for ( uint8_t __i = 0; __i < 2 ; __i++){
	if ( _num && (_tmp_addr) >= _dest_addr){
	  __data += (*_buffer++) << __endian_shift;
	  _num--;
	}
	else{
	  __data += read_byte(_tmp_addr) << __endian_shift;
	}
	__endian_shift ^= 0x08;
	_tmp_addr++;
	_tmp_buffer_fill++;
      }
      boot_page_fill(_tmp_addr - 2, __data);
      // this is strange: filling temporary buffer requires to disable
      // SPMIE bit, according to documentation. 
      SPMCSR = (uint8_t)_BV(SPMIE);
    }
  }

  void
  _irq_page_write_handler()
  {
    // back to page load
    _irq_handler = _irq_page_load_handler; 
    boot_page_write_irq(_dest_addr); 
    // prepare page load 
    _tmp_buffer_fill = 0;
    _dest_addr = _tmp_addr;
  }

  void
  _irq_exit_handler()
  {
    // disable irq, as everything is done
    SPMCSR &= ~((uint8_t)_BV(SPMIE));
    // notify listener, if registered
    if ( _cb ){
      (*_cb)();
      // hang out listener
      _cb = NULL;
    }
  }


  ISR(SPM_READY_vect)
  {
    // wait for eeprom to become ready. looks strange here, but
    // could take max 8.5ms plus 2 cycles (page 23)
    while( ! eeprom_is_ready());

    //call handler
    (*_irq_handler)();
  }

} // namespace flash
