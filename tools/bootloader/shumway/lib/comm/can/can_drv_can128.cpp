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

/*!
 * \file can_drv_can128.cpp
 * \brief MCU specific CAN driver.
 *
 * Definitions of a CAN driver for the AT90CAN128 mcu.
 */

#include "can_drv_can128.h"

#include <stddef.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

  
namespace can
{
  /*!
   * Global callback for interrupt service routine.
   */
  void (*_cb)(const cmd_t& ) = NULL;

  void
  init(enum CONF_CANBT1 canbt1, 
       enum CONF_CANBT2 canbt2, 
       enum CONF_CANBT3 canbt3){
    
    // this is more board stuff .. maybe better there next time
    DDRD |= 0x80;
    PORTD = 0;

    CANGCON = (1<<SWRES);
    clear_all_mobs();
    set_baudrate(canbt1, canbt2, canbt3);
    enable();
  }

  void 
  send(const cmd_t& cmd)
  {
    select_mob(cmd.mob_id);
    set_can_id(cmd.id, cmd.ide);
    for (uint8_t __i = 0; __i < cmd.dlc; __i++){
      CANMSG = cmd.data[__i];
    }
    set_dlc(cmd.dlc);
    enable_tx();
    wait_for_tx_rdy();
    re_enable_tx();
  }

  bool 
  register_rx(MOb mob_id,
	      void (*cb)(const cmd_t& cmd))
  {
    if ( ! _cb ){
      select_mob(mob_id);
      // clear status
      clear_status();
      // enable reception
      enable_rx();
      //enable recv interrupts
      enable_rx_irq();
      // enable irq for mob
      enable_mob_irq(mob_id);
      //wait until module ready
      wait_for_rdy();
      // set function pointer
      _cb = cb;
      // enable global irqs
      sei();
      return false;
    }
    return true;
  }

  void 
  unregister_rx(MOb mob_id){
    select_mob(mob_id);
    // disable recv interrupts
    disable_rx_irq();
    // disable irq for mob
    disable_mob_irq(mob_id);
    _cb = NULL;
  }

  void enable_msk(const cmd_t& cmd)
  {
    id_t __id;
    __id.ext = ~0;

    select_mob(cmd.mob_id);
    set_can_id(cmd.id, cmd.ide);
    set_msk(__id, cmd.ide);
    disable_rtr_msk();
    if ( cmd.ide ){
      enable_ide_msk();
    }
  }

  void disable_msk(const cmd_t& cmd)
  {
    id_t __id;
    __id.ext = 0;

    select_mob(cmd.mob_id);
    set_msk(__id, cmd.ide);
    disable_ide_msk();
  }

  void set_can_id(const id_t& id, bool extended)
  {
    id_t __id = id;
    if ( extended ){
      __id.ext <<= 3;
      CANIDT4 = __id.tab[0];
      CANIDT3 = __id.tab[1];
      CANIDT2 = __id.tab[2];
      CANIDT1 = __id.tab[3];
      CANCDMOB |= (1<<IDE);
    }
    else{
      __id.std <<= 5;
      CANIDT2 = __id.tab[0];
      CANIDT1 = __id.tab[1];
    }
  }

  void set_msk(const id_t& id, bool extended)
  {
    id_t __id = id;
    if ( extended ){
      __id.ext <<= 3;
      CANIDM4 = __id.tab[0];
      CANIDM3 = __id.tab[1];
      CANIDM2 = __id.tab[2];
      CANIDM1 = __id.tab[3];
      CANCDMOB |= (1<<IDE);
    }
    else{
      __id.std <<= 5;
      CANIDM2 = __id.tab[0];
      CANIDM1 = __id.tab[1];
    }
  }


  /*!
   * \brief ISR for CAN port.
   *
   * Interrupt service routine of CAN transfer interrupt.
   */
  ISR(CANIT_vect)
  {
    uint8_t __cp_save = CANPAGE;
    cmd_t __cmd;

    __cmd.mob_id = get_mob();
    // set appropriate mob and read data
    select_mob(__cmd.mob_id);
    // read-modify-write, see manual page 248
    CANSTMOB &= ~(1<<RXOK);
    for (uint8_t __i = 0; __i < ( CANCDMOB & 0x0F); __i++){
      __cmd.data[__i] = CANMSG;
    }
    __cmd.dlc = get_dlc();
    __cmd.ide = is_extended_id();;
    get_can_id(__cmd.id);

    //re-enable transmission
    clear_status();
    enable_rx();

    // call listener. this late, as there could possibly made something nasty
    (*_cb)(__cmd);
    CANPAGE = __cp_save;
  }

} // namespace can





