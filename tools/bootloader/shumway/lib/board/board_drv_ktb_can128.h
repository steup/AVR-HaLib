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

/*$Id: board_drv_ktb_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file board_drv_ktb_can128.h
 * \brief Declarations of board specific functions.
 *
 * The common case is a mcu settled on a board. Therefore mcu behaviour may be
 * influenced by board behaviour. 
 */
#ifndef __BOARD_DRV_KTB_CAN128_H
#define __BOARD_DRV_KTB_CAN128_H

#include <stdint.h>
#include <avr/io.h>


/*!
 * \namespace board
 * \brief Board functions.
 *
 * Encapsulates board functions. Prevents possible name clashing.
 */
namespace board
{

  /*!
   * \brief Get board id.
   *
   * A board provides a (maybe unique) identifier.
   *
   * \return Board identifier.
   */
  static inline uint32_t get_own_id(){
//	DDRG &= ~( (1<<PG0) | (1<<PG1) );
//	PORTG = (1<<PG0) | (1<<PG1);
//	return (PING &0x3) + 0x200;
	return 199;
  }

  /*!
   * \brief Enables a condition based on time counting.
   *
   * A board provides a condition related to the flow of time.
   *
   * \param[in] cb Callback in case of a timeout.
   * \param[in] res Resolution of time. Specifies count period as well. 
   * For instance, resolution of 1 waits a second, 2 a half second, 10 waits
   * 100 milliseconds and so on.
   *
   * \return True On an error. \n
   * False On success.
   */
  bool enable_timed_condition(void (*cb)(), uint8_t res);

  /*!
   * \brief Disables timer condition.
   *
   * Disable waiting for timer condition.
   */
  void disable_timed_condition();

} // namespace board


#endif /* __BOARD_DRV_KTB_CAN128_H */
