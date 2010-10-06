/*! \addtogroup atmega128rfa1 */
/*@{*/
/*! \file   include/avr-halib/avr/interrupts/atmega128rfa1.h
 *  \brief  Defines atmega128rfa1 interrupt classes
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */
#pragma once

#include "avr-halib/avr/interrupt.h"
#if 0
DefineInterrupt( SIG_INTERRUPT0 );          // External Interrupt 0
DefineInterrupt( SIG_INTERRUPT1 );          // External Interrupt 1
DefineInterrupt( SIG_INTERRUPT2 );          // External Interrupt 2
DefineInterrupt( SIG_INTERRUPT3 );          // External Interrupt 3
DefineInterrupt( SIG_INTERRUPT4 );          // External Interrupt 4
DefineInterrupt( SIG_INTERRUPT5 );          // External Interrupt 5
DefineInterrupt( SIG_INTERRUPT6 );          // External Interrupt 6
DefineInterrupt( SIG_INTERRUPT7 );          // External Interrupt 7
DefineInterrupt( SIG_PIN_CHANGE0 );         // Pin Change Interrupt Request 0
DefineInterrupt( SIG_PIN_CHANGE1 );         // Pin Change Interrupt Request 1
DefineInterrupt( SIG_PIN_CHANGE2 );         // Pin Change Interrupt Request 2
DefineInterrupt( SIG_ADC );                 // ADC Conversion Complete
DefineInterrupt( SIG_COMPARATOR );          // Analog Comparator
DefineInterrupt( SIG_EEPROM_READY );        // EEPROM Ready
DefineInterrupt( SIG_SPI );                 // Serial Transfer Complete
DefineInterrupt( SIG_SPM_READY );           // Store Program Memory Read
DefineInterrupt( SIG_2WIRE_SERIAL );        // 2-wire Serial Interface
DefineInterrupt( SIG_USART0_RECV );          // USART0 Rx Complete
DefineInterrupt( SIG_USART0_TRANS );         // USART0 Tx Complete
DefineInterrupt( SIG_USART0_DATA );          // USART0 Data Register Empty
DefineInterrupt( SIG_USART1_RECV );          // USART1 Rx Complete
DefineInterrupt( SIG_USART1_TRANS );         // USART1 Tx Complete
DefineInterrupt( SIG_USART1_DATA );          // USART1 Data Register Empty
DefineInterrupt( SIG_WATCHDOG_TIMEOUT );    // Watchdog Timeout Interrupt
DefineInterrupt( SIG_OUTPUT_COMPARE0A );    // Timer/Counter0 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE0B );    // Timer/Counter0 Compare Match B
DefineInterrupt( SIG_OVERFLOW0 );           // Timer/Counter0 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE1 );      // Timer/Counter1 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE1A );    // Timer/Counter1 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE1B );    // Timer/Counter1 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE1C );    // Timer/Counter1 Compare Match C
DefineInterrupt( SIG_OVERFLOW1 );           // Timer/Counter1 Overflow
DefineInterrupt( SIG_OUTPUT_COMPARE2A );    // Timer/Counter2 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE2B );    // Timer/Counter2 Compare Match A
DefineInterrupt( SIG_OVERFLOW2 );           // Timer/Counter2 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE3 );      // Timer/Counter3 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE3A );    // Timer/Counter3 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE3B );    // Timer/Counter3 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE3C );    // Timer/Counter3 Compare Match C
DefineInterrupt( SIG_OVERFLOW3 );           // Timer/Counter3 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE4 );      // Timer/Counter4 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE4A );    // Timer/Counter4 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE4B );    // Timer/Counter4 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE4C );    // Timer/Counter4 Compare Match C
DefineInterrupt( SIG_OVERFLOW4 );           // Timer/Counter4 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE5 );      // Timer/Counter5 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE5A );    // Timer/Counter5 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE5B );    // Timer/Counter5 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE5C );    // Timer/Counter5 Compare Match C
DefineInterrupt( SIG_OVERFLOW5 );           // Timer/Counter5 Overflow
#endif
/* External Interrupt Request 0 */
DefineInterrupt(  INT0_vect                       );
/* External Interrupt Request 1 */
DefineInterrupt(  INT1_vect                       );
/* External Interrupt Request 2 */
DefineInterrupt(  INT2_vect                       );
/* External Interrupt Request 3 */
DefineInterrupt(  INT3_vect                       );
/* External Interrupt Request 4 */
DefineInterrupt(  INT4_vect                       );
/* External Interrupt Request 5 */
DefineInterrupt(  INT5_vect                       );
/* External Interrupt Request 6 */
DefineInterrupt(  INT6_vect                       );
/* External Interrupt Request 7 */
DefineInterrupt(  INT7_vect                       );
/* Pin Change Interrupt Request 0 */
DefineInterrupt(  PCINT0_vect                     );
/* Pin Change Interrupt Request 1 */
DefineInterrupt(  PCINT1_vect                     );
/* Pin Change Interrupt Request 2 */
DefineInterrupt(  PCINT2_vect                     );
/* Watchdog Time-out Interrupt */
DefineInterrupt(  WDT_vect                        );
/* Timer/Counter2 Compare Match A */
DefineInterrupt(  TIMER2_COMPA_vect               );
/* Timer/Counter2 Compare Match B */
DefineInterrupt(  TIMER2_COMPB_vect               );
/* Timer/Counter2 Overflow */
DefineInterrupt(  TIMER2_OVF_vect                 );
/* Timer/Counter1 Capture Event */
DefineInterrupt(  TIMER1_CAPT_vect                );
/* Timer/Counter1 Compare Match A */
DefineInterrupt(  TIMER1_COMPA_vect               );
/* Timer/Counter1 Compare Match B */
DefineInterrupt(  TIMER1_COMPB_vect               );
/* Timer/Counter1 Compare Match C */
DefineInterrupt(  TIMER1_COMPC_vect               );
/* Timer/Counter1 Overflow */
DefineInterrupt(  TIMER1_OVF_vect                 );
/* Timer/Counter0 Compare Match A */
DefineInterrupt(  TIMER0_COMPA_vect               );
/* Timer/Counter0 Compare Match B */
DefineInterrupt(  TIMER0_COMPB_vect               );
/* Timer/Counter0 Overflow */
DefineInterrupt(  TIMER0_OVF_vect                 );
/* SPI Serial Transfer Complete */
DefineInterrupt(  SPI_STC_vect                    );
/* USART0, Rx Complete */
DefineInterrupt(  USART0_RX_vect                  );
/* USART0 Data register Empty */
DefineInterrupt(  USART0_UDRE_vect                );
/* USART0, Tx Complete */
DefineInterrupt(  USART0_TX_vect                  );
/* Analog Comparator */
DefineInterrupt(  ANALOG_COMP_vect                );
/* ADC Conversion Complete */
DefineInterrupt(  ADC_vect                        );
/* EEPROM Ready */
DefineInterrupt(  EE_READY_vect                   );
/* Timer/Counter3 Capture Event */
DefineInterrupt(  TIMER3_CAPT_vect                );
/* Timer/Counter3 Compare Match A */
DefineInterrupt(  TIMER3_COMPA_vect               );
/* Timer/Counter3 Compare Match B */
DefineInterrupt(  TIMER3_COMPB_vect               );
/* Timer/Counter3 Compare Match C */
DefineInterrupt(  TIMER3_COMPC_vect               );
/* Timer/Counter3 Overflow */
DefineInterrupt(  TIMER3_OVF_vect                 );
/* USART1, Rx Complete */
DefineInterrupt(  USART1_RX_vect                  );
/* USART1 Data register Empty */
DefineInterrupt(  USART1_UDRE_vect                );
/* USART1, Tx Complete */
DefineInterrupt(  USART1_TX_vect                  );
/* 2-wire Serial Interface */
DefineInterrupt(  TWI_vect                        );
/* Store Program Memory Read */
DefineInterrupt(  SPM_READY_vect                  );
/* Timer/Counter4 Capture Event */
DefineInterrupt(  TIMER4_CAPT_vect                );
/* Timer/Counter4 Compare Match A */
DefineInterrupt(  TIMER4_COMPA_vect               );
/* Timer/Counter4 Compare Match B */
DefineInterrupt(  TIMER4_COMPB_vect               );
/* Timer/Counter4 Compare Match C */
DefineInterrupt(  TIMER4_COMPC_vect               );
/* Timer/Counter4 Overflow */
DefineInterrupt(  TIMER4_OVF_vect                 );
/* Timer/Counter5 Capture Event */
DefineInterrupt(  TIMER5_CAPT_vect                );
/* Timer/Counter5 Compare Match A */
DefineInterrupt(  TIMER5_COMPA_vect               );
/* Timer/Counter5 Compare Match B */
DefineInterrupt(  TIMER5_COMPB_vect               );
/* Timer/Counter5 Compare Match C */
DefineInterrupt(  TIMER5_COMPC_vect               );
/* Timer/Counter5 Overflow */
DefineInterrupt(  TIMER5_OVF_vect                 );
/* USART2, Rx Complete */
DefineInterrupt(  USART2_RX_vect                  );
/* USART2 Data register Empty */
DefineInterrupt(  USART2_UDRE_vect                );
/* USART2, Tx Complete */
DefineInterrupt(  USART2_TX_vect                  );
/* USART3, Rx Complete */
DefineInterrupt(  USART3_RX_vect                  );
/* USART3 Data register Empty */
DefineInterrupt(  USART3_UDRE_vect                );
/* USART3, Tx Complete */
DefineInterrupt(  USART3_TX_vect                  );
/* TRX24 - PLL lock interrupt */
DefineInterrupt(  TRX24_PLL_LOCK_vect             );
/* TRX24 - PLL unlock interrupt */
DefineInterrupt(  TRX24_PLL_UNLOCK_vect           );
/* TRX24 - Receive start interrupt */
DefineInterrupt(  TRX24_RX_START_vect             );
/* TRX24 - RX_END interrupt */
DefineInterrupt(  TRX24_RX_END_vect               );
/* TRX24 - CCA/ED done interrupt */
DefineInterrupt(  TRX24_CCA_ED_DONE_vect          );
/* TRX24 - XAH - AMI */
DefineInterrupt(  TRX24_XAH_AMI_vect              );
/* TRX24 - TX_END interrupt */
DefineInterrupt(  TRX24_TX_END_vect               );
/* TRX24 AWAKE - tranceiver is reaching state TRX_OFF */
DefineInterrupt(  TRX24_AWAKE_vect                );
/* Symbol counter - compare match 1 interrupt */
DefineInterrupt(  SCNT_CMP1_vect                  );
/* Symbol counter - compare match 2 interrupt */
DefineInterrupt(  SCNT_CMP2_vect                  );
/* Symbol counter - compare match 3 interrupt */
DefineInterrupt(  SCNT_CMP3_vect                  );
/* Symbol counter - overflow interrupt */
DefineInterrupt(  SCNT_OVFL_vect                  );
/* Symbol counter - backoff interrupt */
DefineInterrupt(  SCNT_BACKOFF_vect               );
/* AES engine ready interrupt */
DefineInterrupt(  AES_READY_vect                  );
/* Battery monitor indicates supply voltage below threshold */
DefineInterrupt(  BAT_LOW_vect                    );

/*@}*/
