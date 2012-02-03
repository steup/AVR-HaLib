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

/*!
 * \file config.h
 * \brief Configuration settings.
 *
 * Configuration settings in here.
 */

/*$Id: config.h 484 2010-07-01 13:25:33Z mschulze $*/

#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>

#include "types.h"
#include "protocol.h"
#include "can_drv.h"

/*
  In order to save space, creation of virtual destructors can easily 
  enabled or disabled by this.
 */

/* #define V_DEFAULT_DESTRUCTOR(x) virtual ~x(){}	\ */
/*   void operator delete(void*){} */
/* #define DEFAULT_DESTRUCTOR(x) ~x(){} */

#define V_DEFAULT_DESTRUCTOR(x) 
#define DEFAULT_DESTRUCTOR(x)

/*
 * \brief Gerneral setttings.
 *
 * Enumeration with gerneral configuration settings.
 */
enum GENERAL_SETTING{
  START_CONDITION_TIME = 4, /*!< Time in sec before start application. */
  MAX_SIGNAL_NUM = 20, /*!< Max number of actual exisiting signals. */
  MAX_SERVER_NUM = 4, /*!< Max number of servers used. */
  SHM_NUM = 0x0A, /*!< Number of elements in shared memory. */
  SHM_SIZE = 0xFF, /*!< Size of shared memory elements. */
};

/*
  CAN_SETTING: baudrate settings, according to can controller description
*/
const CONF_CANBT1 BAUDRATE_SETTING_1 = CANBT1_0E; /*!< CAN baudrate register 
						    1 setting. 
						    \todo Direct computation.
						  */
const CONF_CANBT2 BAUDRATE_SETTING_2 = CANBT2_04; /*!< CAN baudrate register 
						    2 setting. 
						    \todo Direct computation.

						  */
const CONF_CANBT3 BAUDRATE_SETTING_3 = CANBT3_13; /*!< CAN baudrate register 
						    3 setting. 
						    \todo Direct computation.
						  */

/*
 * \brief Signals.
 *
 * Enumeration with signals used in communication between servers.
 */
enum SIGNAL{
  SIGNAL_INVALID = 0x00, /*!< Invalid signal. */
  SIGNAL_DEBUG, /*!< Signal to debug something.. */

  SIGNAL_PHY_IN, /*!< Signal for physical-in data. */
  SIGNAL_ID_FILTER_ENABLED, /*!< Signal id filter is enabled. */
  SIGNAL_APPL_START, /*!< Signal application start */

  SIGNAL_COMM_CLOSE, /*!< Signal communication is going to be finished. */
  SIGNAL_ID_OUT,/*!< Signal outgoing data. */
  SIGNAL_ENABLE_ID_FILTER, /*!< Signal to enable id filter. */
  SIGNAL_ID_IN, /*!< Signal for incoming data. */

  SIGNAL_COMM_CLOSED, /*!< Signal communication has ended. */
  SIGNAL_ISP_OUT, /*!< Signal isp output data. */

  SIGNAL_TIMER_OUT, /*!< Signal timer timed out */
  SIGNAL_DISABLE_APPL_START, /*!< Signal to disable automatic application start */
};






#endif /* __CONFIG_H */
