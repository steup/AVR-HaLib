#pragma once


/* External Interrupt Request 0 */
#define SIG_INTERRUPT0 INT0_vect
/* External Interrupt Request 1 */
#define SIG_INTERRUPT1 INT1_vect
/* External Interrupt Request 2 */
#define SIG_INTERRUPT2 INT2_vect
/* External Interrupt Request 3 */
#define SIG_INTERRUPT3 INT3_vect
/* External Interrupt Request 4 */
#define SIG_INTERRUPT4 INT4_vect
/* External Interrupt Request 5 */
#define SIG_INTERRUPT5 INT5_vect
/* External Interrupt Request 6 */
#define SIG_INTERRUPT6  INT6_vect
/* External Interrupt Request 7 */
#define SIG_INTERRUPT7 INT7_vect
/* Pin Change Interrupt Request 0 */
#define SIG_PIN_CHANGE0 PCINT0_vect
/* Pin Change Interrupt Request 1 */
#define SIG_PIN_CHANGE1 PCINT1_vect
/* Pin Change Interrupt Request 2 */
#define SIG_PIN_CHANGE2 PCINT2_vect
/* Watchdog Time-out Interrupt */
#define SIG_WATCHDOG_TIMEOUT WDT_vect
/* Timer/Counter2 Compare Match A */
#define SIG_OUTPUT_COMPARE2A TIMER2_COMPA_vect
/* Timer/Counter2 Compare Match B */
#define SIG_OUTPUT_COMPARE2B TIMER2_COMPB_vect
/* Timer/Counter2 Overflow */
#define SIG_OVERFLOW2 TIMER2_OVF_vect
/* Timer/Counter1 Capture Event */
#define SIG_INPUT_CAPTURE1 TIMER1_CAPT_vect
/* Timer/Counter1 Compare Match A */
#define SIG_OUTPUT_COMPARE1A TIMER1_COMPA_vect
/* Timer/Counter1 Compare Match B */
#define SIG_OUTPUT_COMPARE1B TIMER1_COMPB_vect
/* Timer/Counter1 Compare Match C */
#define SIG_OUTPUT_COMPARE1C TIMER1_COMPC_vect
/* Timer/Counter1 Overflow */
#define SIG_OVERFLOW1 TIMER1_OVF_vect
/* Timer/Counter0 Compare Match A */
#define SIG_OUTPUT_COMPARE0A TIMER0_COMPA_vect
/* Timer/Counter0 Compare Match B */
#define SIG_OUTPUT_COMPARE0B TIMER0_COMPB_vect
/* Timer/Counter0 Overflow */
#define SIG_OVERFLOW0 TIMER0_OVF_vect
/* SPI Serial Transfer Complete */
#define SIG_SPI SPI_STC_vect
/* USART0, Rx Complete */
#define SIG_USART0_RECV USART0_RX_vect
/* USART0 Data register Empty */
#define SIG_USART0_DATA USART0_UDRE_vect
/* USART0, Tx Complete */
#define SIG_USART0_TRANS USART0_TX_vect
/* Analog Comparator */
#define SIG_COMPARATOR ANALOG_COMP_vect
/* ADC Conversion Complete */
#define SIG_ADC ADC_vect
/* EEPROM Ready */
#define SIG_EEPROM_READY EE_READY_vect
/* Timer/Counter3 Capture Event */
#define SIG_INPUT_CAPTURE3 TIMER3_CAPT_vect
/* Timer/Counter3 Compare Match A */
#define SIG_OUTPUT_COMPARE3A TIMER3_COMPA_vect
/* Timer/Counter3 Compare Match B */
#define SIG_OUTPUT_COMPARE3B TIMER3_COMPB_vect
/* Timer/Counter3 Compare Match C */
#define SIG_OUTPUT_COMPARE3C TIMER3_COMPC_vect
/* Timer/Counter3 Overflow */
#define SIG_OVERFLOW3 TIMER3_OVF_vect
/* USART1, Rx Complete */
#define SIG_USART1_RECV USART1_RX_vect
/* USART1 Data register Empty */
#define SIG_USART1_DATA USART1_UDRE_vect
/* USART1, Tx Complete */
#define SIG_USART1_TRANS USART1_TX_vect
/* 2-wire Serial Interface */
#define SIG_2WIRE_SERIAL TWI_vect
/* Store Program Memory Read */
#define SIG_SPM_READY SPM_READY_vect
/* Timer/Counter4 Capture Event */
#define SIG_INPUT_CAPTURE4 TIMER4_CAPT_vect
/* Timer/Counter4 Compare Match A */
#define SIG_OUTPUT_COMPARE4A TIMER4_COMPA_vect
/* Timer/Counter4 Compare Match B */
#define SIG_OUTPUT_COMPARE4B TIMER4_COMPB_vect
/* Timer/Counter4 Compare Match C */
#define SIG_OUTPUT_COMPARE4C TIMER4_COMPC_vect
/* Timer/Counter4 Overflow */
#define SIG_OVERFLOW4 TIMER4_OVF_vect
/* Timer/Counter5 Capture Event */
#define SIG_INPUT_CAPTURE5 TIMER5_CAPT_vect
/* Timer/Counter5 Compare Match A */
#define SIG_OUTPUT_COMPARE5A TIMER5_COMPA_vect
/* Timer/Counter5 Compare Match B */
#define SIG_OUTPUT_COMPARE5B TIMER5_COMPB_vect
/* Timer/Counter5 Compare Match C */
#define SIG_OUTPUT_COMPARE5C TIMER5_COMPC_vect
/* Timer/Counter5 Overflow */
#define SIG_OVERFLOW5 TIMER5_OVF_vect
/* USART2, Rx Complete */
#define SIG_USART2_RECV USART2_RX_vect
/* USART2 Data register Empty */
#define SIG_USART2_DATA USART2_UDRE_vect
/* USART2, Tx Complete */
#define SIG_USART2_TRANS USART2_TX_vect
/* USART3, Rx Complete */
#define SIG_USART3_RECV USART3_RX_vect
/* USART3 Data register Empty */
#define SIG_USART3_DATA USART3_UDRE_vect
/* USART3, Tx Complete */
#define SIG_USART3_TRANS USART3_TX_vect