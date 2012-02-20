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

/*$Id: can_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file can_drv_can128.h
 * \brief MCU specific CAN driver.
 * 
 * CAN driver for the AT90CAN128 mcu.
 *
 * \note It's just intended (and tested) for the mcu spoken of. It should 
 * be possible to use it for other AVR's too, as they provide the same 
 * interface.
 */

#ifndef __CAN_DRV_CAN128_H
#define __CAN_DRV_CAN128_H

#include <stdint.h>
#include <avr/io.h>

#include "mcu.h"
#include "can.h"
using namespace can;


namespace can
{
  /*!
   * \brief MCU constants.
   *
   * MCU specific properties.
   */
  enum {
    MOb_NUM = 15, /*!< Number of available message objects. */
  };

  /*!
   * \brief MOb identifiers.
   *
   * Message object identifers.
   */
  enum MOb{
    MOb_0 = 0,
    MOb_1, 
    MOb_2, 
    MOb_3, 
    MOb_4, 
    MOb_5, 
    MOb_6, 
    MOb_7, 
    MOb_8, 
    MOb_9, 
    MOb_10,
    MOb_11,
    MOb_12,
    MOb_13,
    MOb_14,
  };

  /*!
   * \brief CANBT1 register settings.
   *
   * Settings for bit timing register CANBT1. See manual at page 264ff.
   * Together with the two other bit timing registers, a baudrate may
   * be specified.
   */
  enum CONF_CANBT1{CANBT1_00 = 0x00,
		   CANBT1_02 = 0x02,
		   CANBT1_04 = 0x04,
		   CANBT1_06 = 0x06,
		   CANBT1_08 = 0x08,
		   CANBT1_0A = 0x0A,
		   CANBT1_0E = 0x0E,
		   CANBT1_12 = 0x12,
		   CANBT1_16 = 0x16,
		   CANBT1_26 = 0x26,
  };

  /*!
   * \brief CANBT2 register settings.
   *
   * Settings for bit timing register CANBT2. See manual at page 264ff.
   * Together with the two other bit timing registers, a baudrate may
   * be specified.
   */
  enum CONF_CANBT2{CANBT2_04 = 0x04,
		   CANBT2_06 = 0x06,
		   CANBT2_08 = 0x08,
		   CANBT2_0C = 0x0C,
		   CANBT2_0E = 0x0E,
  };

  /*!
   * \brief CANBT3 register settings.
   *
   * Settings for bit timing register CANBT3. See manual at page 264ff.
   * Together with the two other bit timing registers, a baudrate may
   * be specified.
   */
  enum CONF_CANBT3{CANBT3_13 = 0x13,
		   CANBT3_23 = 0x23,
		   CANBT3_25 = 0x25,
		   CANBT3_35 = 0x35,
		   CANBT3_37 = 0x37,
		   CANBT3_4B = 0x4B,
  };
	   
  /*!
   * Union for CAN message identifier.
   */
  typedef union{
    uint16_t std;
    uint32_t ext;
    uint8_t tab[4];
  } id_t;

  /*!
   * CAN command. Communication of driver with outside world bases on 
   * this strcutures.
   */
  typedef struct {
    enum MOb mob_id;
    id_t id;
    bool ide;
    uint8_t data[DATA_LEN];
    uint8_t dlc;
  } cmd_t;

  /*!
   * \brief Initialise CAN port.
   * 
   * Performs initialisation of can port. Sets baudrate.
   *
   * \param[in] canbt1 Register setting of bit timing register CANBT1.
   * \param[in] canbt2 Register setting of bit timing register CANBT2.
   * \param[in] canbt3 Register setting of bit timing register CANBT3.
   */
  void init(enum CONF_CANBT1 canbt1, 
	    enum CONF_CANBT2 canbt2, 
	    enum CONF_CANBT3 canbt3);

  /*!
   * \brief Set baudrate.
   *
   * Sets baudrate with the given arguments for the bit timing registers.
   *
   * \param[in] canbt1 Bit setting of bit timing register CANBT1.
   * \param[in] canbt2 Bit setting of bit timing register CANBT2.
   * \param[in] canbt3 Bit setting of bit timing register CANBT3.
   *
   * \todo If informations are available about hardware latencies (e.g.
   * propagation times), this would be more convinient with one real baudrate 
   * value.
   */
  static inline void set_baudrate(enum CONF_CANBT1 canbt1, 
			   enum CONF_CANBT2 canbt2, 
			   enum CONF_CANBT3 canbt3){
    CANBT1 = (uint8_t) canbt1;
    CANBT2 = (uint8_t) canbt2;
    CANBT3 = (uint8_t) canbt3;
  }

  /*!
   * \brief Send data.
   *
   * Sends data via CAN according to the settings given in the argument.
   *
   * \param[in] cmd Reference to CAN command. Contains further details
   * like identifier, dlc and so on.
   */
  void send(const cmd_t& cmd);

  /*!
   * \brief Register for receive of CAN data.
   *
   * Registers for receive of messages from the given message object.
   *
   * \param[in] mob_id Identifier of message object to receive from.
   * \param[in] cb Callback function pointer to use on messages received. 
   *
   * \return True In case of an error.\n
   *   False On success.
   */
  bool register_rx(enum MOb mob_id, void (*cb)(const cmd_t& ));

  /*!
   * \brief Unregister from reception of CAN data.
   *
   * Unregisters for receive of messages from the given message object.
   *
   * \param[in] mob_id Identifier of message object to receive from.
   */
  void unregister_rx(enum MOb mob_id);

  /*!
   * \brief Enable identifier masking.
   *
   * Enables identifier masking. Identifier to use has to be contained
   * in the argument, as well as message object identifer.
   *
   * \param[in] cmd Reference to CAN command. Contains further details
   * like identifier, dlc and so on.
   */
  void enable_msk(const cmd_t& cmd);

  /*!
   * \brief Disable identifier masking.
   *
   * Disables identifier masking. Message object identifer to use has to be 
   * contained in the argument.
   *
   * \param[in] cmd Reference to CAN command. Contains further details
   * like identifier, dlc and so on.
   */
  void disable_msk(const cmd_t& cmd);

  /*!
   * \brief Check for extended identifier.
   *
   * Checks, if extended identifier has to be used.
   *
   * \return True If extended identifiers has to be used.\n
   * False If standard identifiers are sufficient.
   */
  static inline bool is_extended_id(){
    return ( CANCDMOB & (1 << IDE));
  }

  /*!
   * \brief Set CAN identifier.
   *
   * Set CAN identifier. 
   *
   * \param[in] id Reference to identifier union.
   * \param[in] extended Use of extended identifier.
   */
  void set_can_id(const id_t& id, bool extended);

  /*!
   * \brief Get CAN identifier.
   *
   * Get CAN identifier. 
   *
   * \param[out] id Reference to identifier union.
   */
  static inline void get_can_id(id_t& id){
    if ( is_extended_id()){
      id.tab[3] = CANIDT1;
      id.tab[2] = CANIDT2;
      id.tab[1] = CANIDT3;
      id.tab[0] = CANIDT4;
      id.ext >>= 3; 
      // works with little endian. will kick unused bits out of tab[0]
    }
    else{
      id.tab[1] = CANIDT1;
      id.tab[0] = CANIDT2;
      id.std >>= 5;
    }
  }
  /*!
   * \brief Set masking for identifier.
   *
   * Set masking for identifier.
   *
   * \param[in] id Reference to identifier union.
   * \param[in] extended Use of extended identifier.
   */
  void set_msk(const id_t& id, bool extended);

  /*!
   * \brief Enable CAN port.
   *
   * Enable CAN port by setting appropriate bits.
   */
  static inline void enable(){
    CANGCON |= (1<<ENASTB);    
  }

  /*!
   * \brief Disable CAN port.
   *
   * Disable CAN port by setting appropriate bits.
   */
  static inline void disable(){
    CANGCON &= ~(1<<ENASTB);    
  }

  /*!
   * \brief Enable transmission.
   *
   * Enable transmission by setting appropriate bits.
   */
  static inline void enable_tx() {
    CANCDMOB |= (1<<CONMOB0);
  } 

  /*!
   * \brief Enable reception.
   *
   * Enable reception by setting appropriate bits.
   */
  static inline void enable_rx(){
    CANCDMOB |= (1<<CONMOB1);
  }

  /*!
   * \brief Enable receive irq.
   *
   * Enable generation of interrupts on message reception by setting 
   * appropriate bits.
   */
  static inline void enable_rx_irq(){
    CANGIE |= (1<<ENIT) | (1<<ENRX);
  }

  /*!
   * \brief Disable receive irq.
   *
   * Disable generation of interrupts on message reception by setting 
   * appropriate bits.
   */
  static inline void disable_rx_irq(){
    CANGIE &= ~(1<<ENRX);
  }

  /*!
   * \brief Set dlc.
   *
   * Set data lenght code by setting appropriate bits.
   *
   * \param[in] dlc Data lenght code to set.
   */
  static inline void set_dlc(uint8_t dlc){
    CANCDMOB &= 0xF0; // erase dlc bits first.
    CANCDMOB |= ( dlc & 0x0F );
  }

  /*!
   * \brief Get dlc.
   *
   * get data lenght code by reading appropriate bits.
   *
   * \return Data lenght code set.
   */
  static inline uint8_t get_dlc(){
    return CANCDMOB & 0x0F;
  }

  /*!
   * \brief Enable ide mask.
   *
   * Enable masking for extended identifiers by setting appropriate bits.
   */
  static inline void enable_ide_msk(){
    CANIDM4 |= (1<<IDEMSK);
  }

  /*!
   * \brief Disable ide mask.
   *
   * Disable masking for extended identifiers by setting appropriate bits.
   */
  static inline void disable_ide_msk(){
    CANIDM4 &= ~(1<<IDEMSK);
  }

  /*!
   * \brief Enable rtr mask.
   *
   * Enable masking of remote transmission requests by setting appropriate 
   * bits.
   */
  static inline void enable_rtr_msk(){
    CANIDM4 |= (1<<RTRMSK);
  }

  /*!
   * \brief Disable rtr mask.
   *
   * Disable masking of remote transmission requests by setting appropriate 
   * bits.
   */
  static inline void disable_rtr_msk(){
    CANIDM4 &= ~(1<<RTRMSK);
  }

  /*!
   * \brief Select MOb.
   *
   * Selects message object. They are shadowed, at any time only the 
   * selected one is in use, all MOb specific actions are only done on this. 
   *
   * \param[in] mob_id Identifier of message object.
   *
   * \note Uses auto increment index, starts at index = 0.
   */
  static inline void select_mob(MOb mob_id){
    //enable MOb. 
    CANPAGE = ((uint8_t) mob_id) << MOBNB0;
  }

  /*!
   * \brief Get MOb.
   *
   * On interrupts enabled, it has to be determined, which message object
   * signalised an interrupt.
   *
   * \return Identifier of message object, which signalised something.
   * 
   * \todo Uses not very nice enum "cast".
   */
  static inline MOb get_mob(){
    // the problem is to find the mailbox with these one-hot registers
    uint16_t __mob_bit = CANSIT2 + (CANSIT1 << 8);
    uint8_t __i = (uint8_t)MOb_NUM;
    while( __i-- && ! (__mob_bit & (1 << __i)));
    return MOb(__i);
  }

  /*!
   * \brief Enable irq for MOb.
   *
   * Enables interrupt generation for the given message object.
   *
   * \param[in] mob_id Identifier of message object.
   *
   * \note Interrupts have still be enabled globally and for the CAN 
   * controller.
   *
   * \sa enable_rx_irq
   */
  static inline void enable_mob_irq(MOb mob_id){
    CANIE1 |= ((1 << mob_id) >> 8 );
    CANIE2 |= (1 << mob_id);
  }

  /*!
   * \brief Disable irq for MOb.
   *
   * Disables interrupt generation of the given message object.
   *
   * \param[in] mob_id Identifier of message object.
   */
  static inline void disable_mob_irq(MOb mob_id){
    CANIE1 &= ~((1 << mob_id) >> 8 );
    CANIE2 &= ~(1 << mob_id);
  }


  /*!
   * \brief Wait for rdy.
   *
   * Waits until CAN port is ready for further actions.
   */
  static inline void wait_for_rdy(){
    while (!(CANGSTA & (1<<ENFG)));
  }

  /*!
   * \brief Wait for tx rdy.
   *
   * Waits until message object is ready for further transmissions.
   */
  static inline void wait_for_tx_rdy(){
    while (!(CANSTMOB & (1<<TXOK)));
  }

  /*!
   * \brief Re-enable transmission.
   *
   * After reception, this enables receptions of further messages on the
   * actual message object.
   */
  static inline void re_enable_tx(){
    CANSTMOB &= ~(1<<TXOK);                 //reset flag
    CANCDMOB &= 0x30; // clear CONMOBs to allow new configuration (e.g. DLC)!
  }

  /*!
   * \brief Clear status.
   *
   * Clears status of actual message object.
   */
  static inline void clear_status(){
     CANSTMOB = 0;
  }

  /*
   * \brief Clear all MObs.
   *
   * Clears all message objects. Includes data, status, identifier and mask
   * registers.
   */
  static inline void clear_all_mobs(){
    uint8_t __i = (uint8_t) MOb_NUM;
    uint8_t __k;

    // clear all mobs
    while(__i--){
      CANPAGE = (__i << 4);
      CANCDMOB = 0;
      CANSTMOB = 0;
      CANIDT1 = 0;
      CANIDT2 = 0;
      CANIDT3 = 0;
      CANIDT4 = 0;
      CANIDM1 = 0;
      CANIDM2 = 0;
      CANIDM3 = 0;
      CANIDM4 = 0;
      __k = (uint8_t)8;
      while(__k--){
	CANMSG = 0;
      }
    }
  }

} // namespace can

#endif /* __CAN_DRV_CAN128_H */
