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

/*$Id: S_Isp.h 484 2010-07-01 13:25:33Z mschulze $*/

#ifndef __S_ISP_H
#define __S_ISP_H

#include <stdint.h>

#include "mcu_drv.h"
using namespace mcu;
#include "Server.h"
#include "i_header.h"
#include "isp_drv.h"
using namespace isp;

/*!
 * \def _DEBUG_ISP
 *
 * Enables creation of debug messages, if set.
 */
#define _DEBUG_ISP 0

#if _DEBUG_ISP

/*!
 * \def DEBUG_ISP
 *
 * Sends a debug message with the message-string under name of this server.
 */
#define DEBUG_ISP(__dbg_msg) DEBUG("ISP: ", __dbg_msg)
#else

/*!
 * \def DEBUG_ISP
 *
 * Defines no debugging.
 */
#define DEBUG_ISP(__dbg_msg )
#endif /* _DEBUG_ISP */


/*!
 * \class S_Isp
 * \brief Server responsible for in-system-programming operations.
 *
 * Messages for doing ISP operations will be treated here. It's somehow 
 * an interface to the hardware as well, as it manages hardware operations.
 *
 * \note Usage of default copy constructor.
 * \note Usage of default assignment operator
 */
class S_Isp : public Server {

 public:
  /*!
   * \brief Default constructor.
   *
   * Initialises status. Default memory area is FLASH, default memory address 
   * is 0.
   */
  S_Isp() : _isp_mem_area(FLASH), 
    _isp_addr(0),
    _isp_in_progress(false),
    _isp_buffer(NULL),
    _isp_buffer_fill(0){
    _S_isp_irq_handler_id = _id;
  }

  /*!
   * \brief Destructor alias.
   *
   * Defined alias for default destructors.
   *
   * \sa DEFAULT_DESTRUCTOR
   */
  DEFAULT_DESTRUCTOR(S_Isp);

  /*!
   * \brief Reception of messages.
   *
   * Handling of message. \n
   * Several messages will be handled.
   * \li \c SIGNAL_ID_IN From operator, these messages have to be 
   * evaluated in more detail, according to the included protocol action.
   * This requires the most work.
   * \li \c SIGNAL_COMM_CLOSE Request to quit work. Finishes any work left.
   *
   * Messages sent by itself will be discarded. Other messages are 
   * transfered back to mediator. 
   *
   * \param[in] __msg Reference to received message.
   *
   * \sa SIGNAL_ID_IN, SIGNAL_COMM_CLOSE
   */
  void receive(Message& __msg);

  /*!
   * \brief Entry for ISP isr. Signalises completed write operation.
   *
   * This provides the callback for the used ISP driver to an instance of this.
   * For this, instance has to register with it's id, as done in constructor.
   */
  static void S_write_complete(); 

  /*!
   * \brief Entry for ISP isr. Signalises completed erase operation.
   *
   * This provides the callback for the used ISP driver to an instance of this.
   * For this, instance has to register with it's id, as done in constructor.
   */
  static void S_erase_complete(); 

  /*!
   * \brief Instance callback from ISP interrupt. Signalises completed write 
   * operation.
   *
   * Prepares for possible next write operation.
   */
  void write_complete(); 

  /*!
   * \brief Instance callback from ISP interrupt. Signalises completed erase 
   * operation.
   *
   * Prepares for possible following ISP operations.
   */
  void erase_complete(); 

  
 private:
  MEMORY_AREA _isp_mem_area;
  uint32_t _isp_addr;
  bool _isp_in_progress;

  shm_key_t _isp_buffer_key;
  uint8_t *_isp_buffer;
  uint8_t _isp_buffer_size;
  uint8_t _isp_buffer_fill;

  static uint8_t _S_isp_irq_handler_id;


  inline bool _init_isp(){
    {
      bool __rc;
      
      _isp_in_progress = true;
      __rc = write_buffer_async(_isp_mem_area,
				_isp_addr, 
				_isp_buffer, 
				_isp_buffer_fill,
				&S_write_complete);
      if ( __rc ){
	_isp_in_progress = false;
      }
      return __rc;
    }
  }

};

#endif /* __S_ISP_H */
