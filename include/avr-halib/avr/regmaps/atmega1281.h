/*! \addtogroup atmega1281 */
/*@{*/
/*! \file   include/avr-halib/avr/regmaps/atmega1281.h
 *  \brief  Contains atmega1281 specific stuff
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */
#pragma once
#include "avr-halib/avr/interrupt.h"
#include <stdint.h>

/*! \brief  Namespace containing atmega1281 specific stuff
 *
 *  Content in global namespace as well if compiling for atmega1281.
 */
namespace atmega1281
{
    /*! \addtogroup PowerManagement    *//*@{*//*@}*/
    /*! \addtogroup ExternalInterrupts */
    /*@{*/
    /*! \brief      Register map for usage of external interrupts
     *  \ingroup    atmega1281
     *  \ingroup    ExternalInterrupts
     *  \todo       PIN Change Interrupt (PCICR, PCIFR, PCMSK2, PCMSK1, MCMSK0)
     *  \todo       Registeraufbau
     */
    class ExternalInterrupts
    {
        union{
            struct{ _offset :0x3c*8;
                uint8_t efir;
            };
            struct{ _offset :0x3d*8;    // EIMSK (0x3D)
                bool enableInt0 : 1;	///< Enable external interrupt 0
                bool enableInt1 : 1;	///< Enable external interrupt 1
                bool enableInt2 : 1;	///< Enable external interrupt 2
                bool enableInt3 : 1;	///< Enable external interrupt 3
                bool enableInt4 : 1;	///< Enable external interrupt 4
                bool enableInt5 : 1;	///< Enable external interrupt 5
                bool enableInt6 : 1;	///< Enable external interrupt 6
                bool enableInt7 : 1;	///< Enable external interrupt 7
            };
            struct{ _offset :0x6a*8;    // EICRA (0x6a)
                uint8_t senseInt0 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
                uint8_t senseInt1 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
                uint8_t senseInt2 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
                uint8_t senseInt3 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
            };
            struct{ _offset :0x69*8;    // EICRB (0x69)
                uint8_t senseInt4 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
                uint8_t senseInt5 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
                uint8_t senseInt6 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
                uint8_t senseInt7 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
            };
        };
    }__attribute__((packed));
    /*@} ExternalInterrupts --------------------------------------------------- */

    /*! \addtogroup TmerCounter */ /*@{*//*@}*/
    /*! \addtogroup SPI         */
    /*@{*/
    /*! \brief      Register map for usage of the SerialPeripheralInterface
     *  \ingroup    atmega1281
     *  \ingroup    SPI
     */
    template <class _Controller_Configuration = DefineController> class Spi
    {
        public:
            union{
                struct{ _offset :0x4c*8;
                    union{
                        uint8_t spcr;
                        struct{
                            bool    spr0 :1;    ///<
                            bool    spr1 :1;    ///<
                            bool    cpha :1;    ///<
                            bool    cpol :1;    ///<
                            bool    mstr :1;    ///<
                            bool    dord :1;    ///<
                            bool    spe  :1;    ///<
                            bool    spie :1;    ///<
                        };
                    };
                    union{
                        uint8_t spsr;
                        struct{
                            bool    spi2x :1;    ///<
                            bool          :5;    ///< reserved
                            bool    wcol  :1;    ///<
                            bool    spif  :1;    ///<
                        };
                    };
                    uint8_t spdr;
                };
                struct{
                    union{
                        struct{  // pin ss: b 0;
                            uint8_t __pad0 [0x23];
                            bool pin : 1;   // PINB (0x36), bit 0
                            uint8_t  : 7;
                            bool ddr : 1;   // DDRB (0x37), bit 0
                            uint8_t  : 7;
                            bool port : 1;  // PORTB (0x38), bit 0
                        } ss;
                        struct{  // pin sck: b 1;
                            uint8_t __pad0 [0x23];
                            uint8_t  : 1;
                            bool pin : 1;   // PINB (0x36), bit 1
                            uint8_t  : 7;
                            bool ddr : 1;   // DDRB (0x37), bit 1
                            uint8_t  : 7;
                            bool port : 1;  // PORTB (0x38), bit 1
                        } sck;
                        struct{  // pin mosi: b 2;
                            uint8_t __pad0 [0x23];
                            uint8_t  : 2;
                            bool pin : 1;   // PINB (0x36), bit 2
                            uint8_t  : 7;
                            bool ddr : 1;   // DDRB (0x37), bit 2
                            uint8_t  : 7;
                            bool port : 1;  // PORTB (0x38), bit 2
                        } mosi;
                        struct{  // pin miso: b 3;
                            uint8_t __pad0 [0x23];
                            uint8_t  : 3;
                            bool pin : 1;   // PINB (0x36), bit 3
                            uint8_t  : 7;
                            bool ddr : 1;   // DDRB (0x37), bit 3
                            uint8_t  : 7;
                            bool port : 1;  // PORTB (0x38), bit 3
                        } miso;
                    };
                };
            };
            enum {ps2 = 0, ps4 = 1, ps8 = 2, ps16 = 3, ps32 = 4, ps64 = 5, ps128 = 7};
            enum {msb = 0 , lsb = 1};           //first bit
            enum {rising = 0, falling = 1};    //leading edge
            enum {leading = 0 , trailing = 1};  //sample edge
            enum {bussywaitput=true};

            // a way to encapsulate interrupt symbol to use in device specific structure
            // mainly for internal use, syntax not nice at all
            template<class T, void (T::*Fxn)()>
                static void setSpiInterrupt(T & obj)
                {
                    redirectISRM(SIG_SPI, Fxn, obj);
                }
    }__attribute__((packed));
    /*@} SPI ----------------------------------------------------------------- */
    /*! \addtogroup UART        */
    /*@{*/
    /*! \brief      Register map for usage of the UART0
     *  \ingroup    atmega1281
     *  \ingroup    UART
     */
    template <class _Controller_Configuration = DefineController> class _Uart0
    {
        public:
            typedef _Controller_Configuration Controller_Configuration;
            union{
                struct{ _offset :0xc0*8;
                    union{
                        uint8_t ucsra;
                        struct{
                            bool    mpcm :1;    ///<
                            bool    u2x  :1;    ///<
                            bool    pe   :1;    ///<
                            bool    dor  :1;    ///<
                            bool    fe   :1;    ///<
                            bool    udre :1;    ///<
                            bool    txc  :1;    ///<
                            bool    rxc  :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xc1*8;
                    union{
                        uint8_t ucsrb;
                        struct{
                            bool    txb8  :1;    ///<
                            bool    rxb8  :1;    ///<
                            bool    ucsz2 :1;    ///<
                            bool    txen  :1;    ///<
                            bool    rxen  :1;    ///<
                            bool    udrie :1;    ///<
                            bool    txcie :1;    ///<
                            bool    rxcie :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xc2*8;
                    union{
                        uint8_t ucsrc;
                        struct {
                            bool    ucpol :1;    ///<
                            bool    ucsz0 :1;    ///<
                            bool    ucsz1 :1;    ///<
                            bool    usbs  :1;    ///<
                            uint8_t upm   :2;    ///<
                            uint8_t umsel :2;    ///<
                        };
                    };
                };
                struct{ _offset :0xc4*8;
                    uint8_t ubrrl;
                };
                struct{ _offset :0xc5*8;
                    uint8_t ubrrh;
                };
                struct{ _offset :0xc6*8;
                    uint8_t udr;
                };
            };

            // a way to encapsulate interrupt symbol to use in device specific structure
            // mainly for internal use, syntax not nice at all
            template<class T, void (T::*Fxn)()>
                static void setRecvInterrupt(T & obj)
                {
                    redirectISRM(SIG_UART0_RECV, Fxn, obj);
                }

            template<class T, void (T::*Fxn)()>
                static void setDataInterrupt(T & obj)
                {
                    redirectISRM(SIG_UART0_DATA, Fxn, obj);
                }

            typedef class InteruptClass( SIG_UART0_RECV ) RecvInterrupt;
            typedef class InteruptClass( SIG_UART0_DATA ) DataInterrupt;
    }__attribute__((packed));

    /*! \brief      Register map for usage of the UART1
     *  \ingroup    atmega1281
     *  \ingroup    UART
     */
    template <class _Controller_Configuration = DefineController> class _Uart1
    {
        public:
            typedef _Controller_Configuration Controller_Configuration;
            union{
                struct{ _offset :0xc8*8;
                    union{
                        uint8_t ucsra;
                        struct{
                            bool    mpcm :1;    ///<
                            bool    u2x  :1;    ///<
                            bool    pe   :1;    ///<
                            bool    dor  :1;    ///<
                            bool    fe   :1;    ///<
                            bool    udre :1;    ///<
                            bool    txc  :1;    ///<
                            bool    rxc  :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xc9*8;
                    union{
                        uint8_t ucsrb;
                        struct{
                            bool    txb8  :1;    ///<
                            bool    rxb8  :1;    ///<
                            bool    ucsz2 :1;    ///<
                            bool    txen  :1;    ///<
                            bool    rxen  :1;    ///<
                            bool    udrie :1;    ///<
                            bool    txcie :1;    ///<
                            bool    rxcie :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xca*8;
                    union{
                        uint8_t ucsrc;
                        struct {
                            bool    ucpol :1;    ///<
                            bool    ucsz0 :1;    ///<
                            bool    ucsz1 :1;    ///<
                            bool    usbs  :1;    ///<
                            uint8_t upm   :2;    ///<
                            uint8_t umsel :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xcc*8;
                    uint8_t ubrrl;
                };
                struct{ _offset :0xcd*8;
                    uint8_t ubrrh;
                };
                struct{ _offset :0xce*8;
                    uint8_t udr;
                };
            };

            // a way to encapsulate interrupt symbol to use in device specific structure
            // mainly for internal use, syntax not nice at all
            template<class T, void (T::*Fxn)()>
                static void setRecvInterrupt(T & obj)
                {
                    redirectISRM(SIG_UART1_RECV, Fxn, obj);
                }

            template<class T, void (T::*Fxn)()>
                static void setDataInterrupt(T & obj)
                {
                    redirectISRM(SIG_UART1_DATA, Fxn, obj);
                }

            typedef class InteruptClass( SIG_UART1_RECV ) RecvInterrupt;
            typedef class InteruptClass( SIG_UART1_DATA ) DataInterrupt;
    }__attribute__((packed));

    /*! \brief      Register map for usage of the UART2
     *  \ingroup    atmega1281
     *  \ingroup    UART
     */
    template <class _Controller_Configuration = DefineController> class _Uart2
    {
        public:
            typedef _Controller_Configuration Controller_Configuration;
            union{
                struct{ _offset :0xd0*8;
                    union{
                        uint8_t ucsra;
                        struct{
                            bool    mpcm :1;    ///<
                            bool    u2x  :1;    ///<
                            bool    pe   :1;    ///<
                            bool    dor  :1;    ///<
                            bool    fe   :1;    ///<
                            bool    udre :1;    ///<
                            bool    txc  :1;    ///<
                            bool    rxc  :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xd1*8;
                    union{
                        uint8_t ucsrb;
                        struct{
                            bool    txb8  :1;    ///<
                            bool    rxb8  :1;    ///<
                            bool    ucsz2 :1;    ///<
                            bool    txen  :1;    ///<
                            bool    rxen  :1;    ///<
                            bool    udrie :1;    ///<
                            bool    txcie :1;    ///<
                            bool    rxcie :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xd2*8;
                    union{
                        uint8_t ucsrc;
                        struct {
                            bool    ucpol :1;    ///<
                            bool    ucsz0 :1;    ///<
                            bool    ucsz1 :1;    ///<
                            bool    usbs  :1;    ///<
                            uint8_t upm   :2;    ///<
                            uint8_t umsel :1;    ///<
                        };
                    };
                };
                struct{ _offset :0xd4*8;
                    uint8_t ubrrl;
                };
                struct{ _offset :0xd5*8;
                    uint8_t ubrrh;
                };
                struct{ _offset :0xd6*8;
                    uint8_t udr;
                };
            };

            // a way to encapsulate interrupt symbol to use in device specific structure
            // mainly for internal use, syntax not nice at all
            template<class T, void (T::*Fxn)()>
                static void setRecvInterrupt(T & obj)
                {
                    redirectISRM(SIG_USART2_RECV, Fxn, obj);
                }

            template<class T, void (T::*Fxn)()>
                static void setDataInterrupt(T & obj)
                {
                    redirectISRM(SIG_USART2_DATA, Fxn, obj);
                }

            typedef class InteruptClass( SIG_USART2_RECV ) RecvInterrupt;
            typedef class InteruptClass( SIG_USART2_DATA ) DataInterrupt;
    }__attribute__((packed));

    /*! \brief      Register map for usage of the UART3
     *  \ingroup    atmega1281
     *  \ingroup    UART
     */
    template <class _Controller_Configuration = DefineController> class _Uart3
    {
        public:
            typedef _Controller_Configuration Controller_Configuration;
            union{
                struct{ _offset :0x130*8;
                    union{
                        uint8_t ucsra;
                        struct{
                            bool    mpcm :1;    ///<
                            bool    u2x  :1;    ///<
                            bool    pe   :1;    ///<
                            bool    dor  :1;    ///<
                            bool    fe   :1;    ///<
                            bool    udre :1;    ///<
                            bool    txc  :1;    ///<
                            bool    rxc  :1;    ///<
                        };
                    };
                };
                struct{ _offset :0x131*8;
                    union{
                        uint8_t ucsrb;
                        struct{
                            bool    txb8  :1;    ///<
                            bool    rxb8  :1;    ///<
                            bool    ucsz2 :1;    ///<
                            bool    txen  :1;    ///<
                            bool    rxen  :1;    ///<
                            bool    udrie :1;    ///<
                            bool    txcie :1;    ///<
                            bool    rxcie :1;    ///<
                        };
                    };
                };
                struct{ _offset :0x132*8;
                    union{
                        uint8_t ucsrc;
                        struct {
                            bool    ucpol :1;    ///<
                            bool    ucsz0 :1;    ///<
                            bool    ucsz1 :1;    ///<
                            bool    usbs  :1;    ///<
                            uint8_t upm   :2;    ///<
                            uint8_t umsel :1;    ///<
                        };
                    };
                };
                struct{ _offset :0x134*8;
                    uint8_t ubrrl;
                };
                struct{ _offset :0x135*8;
                    uint8_t ubrrh;
                };
                struct{ _offset :0x136*8;
                    uint8_t udr;
                };
            };

            // a way to encapsulate interrupt symbol to use in device specific structure
            // mainly for internal use, syntax not nice at all
            template<class T, void (T::*Fxn)()>
                static void setRecvInterrupt(T & obj)
                {
                    redirectISRM(SIG_USART3_RECV, Fxn, obj);
                }

            template<class T, void (T::*Fxn)()>
                static void setDataInterrupt(T & obj)
                {
                    redirectISRM(SIG_USART3_DATA, Fxn, obj);
                }

            typedef class InteruptClass( SIG_USART3_RECV ) RecvInterrupt;
            typedef class InteruptClass( SIG_USART3_DATA ) DataInterrupt;
    }__attribute__((packed));

    /*!
     *  \ingroup    atmega1281
     *  \ingroup    UART
     */
    template< class _Uart = _Uart0<> > class _Uart_commons: public _Uart
    {
        public:
            enum{noParity=0x00,evenParity=0x2,oddParity=0x3};
            typedef class _Uart::Controller_Configuration Controller_Configuration;

            template<uint8_t databits,char parity,uint8_t stopbits, bool syncronous> inline void configure()
            {
                _Uart::ucsrc  = 0;
                _Uart::umsel  = syncronous;
                _Uart::ucsz2  = (databits==9);
                _Uart::ucsz1  = (databits>6);
                _Uart::ucsz0  = (databits != 5 && databits != 7);
                _Uart::usbs   = (stopbits==2);
                _Uart::upm    = parity=='N'?(noParity):(parity=='E'?(evenParity):(parity=='O'?(oddParity):parity));
            }

            template<uint8_t databits,char parity,uint8_t stopbits> inline void configure()
            {
                _Uart::ucsrc  = 0;
                _Uart::umsel  = false;
                _Uart::ucsz2  = (databits==9);
                _Uart::ucsz1  = (databits>6);
                _Uart::ucsz0  = (databits != 5 && databits != 7);
                _Uart::usbs   = (stopbits==2);
                _Uart::upm    = parity=='N'?(noParity):(parity=='E'?(evenParity):(parity=='O'?(oddParity):parity));
            }

        private:
            inline void setubrr(uint16_t Ubrr) __attribute__ ((always_inline))
            {
                union{
                    uint16_t ubrr;
                    struct{
                        uint8_t ubrrl;
                        uint8_t ubrrh;
                    };
                }ub;
                ub.ubrr = Ubrr;
                _Uart::ubrrh=ub.ubrrh;
                _Uart::ubrrl=ub.ubrrl;
            }
        public:
            inline void setbaudrate(uint32_t baudrate)__attribute__ ((always_inline))
            {
                setubrr(((uint16_t)(Controller_Configuration::controllerClk/8/baudrate)-1)/2);
            }
            inline void setbaudrateU2X(uint32_t baudrate)__attribute__ ((always_inline))
            {
                setubrr(((uint16_t)(Controller_Configuration::controllerClk/4/baudrate)-1)/2);
            }
    };

    template  <class _CC = DefineController, int baud=19200> class Uart0: public _Uart_commons<_Uart0<_CC> >{public:enum{baudrate=baud};};
    template  <class _CC = DefineController, int baud=19200> class Uart1: public _Uart_commons<_Uart1<_CC> >{public:enum{baudrate=baud};};
    template  <class _CC = DefineController, int baud=19200> class Uart2: public _Uart_commons<_Uart2<_CC> >{public:enum{baudrate=baud};};
    template  <class _CC = DefineController, int baud=19200> class Uart3: public _Uart_commons<_Uart3<_CC> >{public:enum{baudrate=baud};};
    /*@} UART ----------------------------------------------------------------- */
    /*! \addtogroup TWI         */
    /*@{*/
    /*! \brief      Register map for usage of the 2-wired-Interface (I2C)
     *  \ingroup    atmega1281
     *  \ingroup    TWI
     */
    template <class _Controller_Configuration = DefineController> class TWI
    {
        public:
            typedef _Controller_Configuration Controller_Configuration;
            union{
                struct{
                    uint8_t __base [0xb8];
                    uint8_t twbr;
                    union{
                        uint8_t twsr;
                        struct{
                            uint8_t twps :2;    ///<
                            uint8_t      :1;    ///<
                            uint8_t tws  :5;    ///<
                        };
                    };
                    union{
                        uint8_t twar;
                        struct{
                            bool    twgce :1;   ///<
                            uint8_t twa   :7;   ///<
                        };
                    };
                    uint8_t twdr;
                    union{
                        uint8_t twcr;
                        struct{
                            bool twie  :1;   ///<
                            bool       :1;   ///<
                            bool twen  :1;   ///<
                            bool twwc  :1;   ///<
                            bool twsto :1;   ///<
                            bool twsta :1;   ///<
                            bool twea  :1;   ///<
                            bool twint :1;   ///<
                        };
                    };
                };
            };

            enum {ps1 = 0, ps4 = 1, ps16 = 2, ps64 = 3};
            enum {read = 1 , write = 0};
            enum status{
                st_start                = 1,   ///<
                repeat_start            = 2,   ///<
                sla_write_ack           = 3,   ///<
                sla_write_noack         = 4,   ///<
                m_data_tx_ack           = 5,   ///<
                m_data_tx_noack         = 6,   ///<
                sla_w_buslost           = 7,   ///<
                sla_read_ack            = 8,   ///<
                sla_read_noack          = 9,   ///<
                m_data_rx_ack           = 10,   ///<
                m_data_rx_noack         = 11,   ///<
                sla_w_recived           = 12,   ///<
                buslost_sla_w_recived   = 13,   ///<
                gc_w_recived            = 14,   ///<
                buslost_gc_w_recived    = 15,   ///<
                sl_data_rx_ack          = 16,   ///<
                sl_data_rx_noack        = 17,   ///<
                gc_data_rx_ack          = 18,   ///<
                gc_data_rx_noack        = 19,   ///<
                stop_recived            = 20,   ///<
                sla_r_recived           = 21,   ///<
                buslost_sla_r_recived   = 22,   ///<
                sl_data_tx_ack          = 23,   ///<
                sl_data_tx_noack        = 24,   ///<
                sl_data_tx_ack_last     = 25,   ///<
                no_status               = 31,   ///<
                bus_error               = 0     ///<
            };

            // a way to encapsulate interrupt symbol to use in device specific structure
            // mainly for internal use, syntax not nice at all
            template<class T, void (T::*Fxn)()>
                static void setTwiInterrupt(T & obj)
                {
                    redirectISRM(SIG_2WIRE_SERIAL, Fxn, obj);
                }
    }__attribute__((packed));
    /*@} TWI ----------------------------------------------------------------- */
    /*! \addtogroup ADConv      */
    /*@{*/

    //Begin ADconv
    /*! \brief  Register map for ADConv of atmega1281
     *  \param  Controller_Configuration    TODO
     *  \ingroup    atmega1281
     *  \ingroup    ADConv
     */
    template <class Controller_Configuration> struct ADConv
    {
        private:    _offset :0x78*8;
        public:
         union
         {
             struct
             {
                 uint8_t adcl:8;
                 uint8_t adch:8;
             };
             struct
             {
                 uint16_t adc:16;
             };
         };
         union
         {
             uint8_t adcsra:8;
             struct
             {
                 uint8_t    adps : 3;
                 bool       adie : 1;
                 bool       adif : 1;
                 bool       adate: 1;
                 bool       adsc : 1;
                 bool       aden : 1;
            };
         };
         union
         {
             uint8_t adcsrb:8;
             struct
             {
                 uint8_t    adts : 3;
                 bool       mux5 : 1;
                 uint8_t         : 2;
                 bool       acme : 1;
                 bool            : 1;
             };
         };
         union
         {
             uint8_t  admux:8;
             struct{
                 uint8_t    mux     :5;
                 bool       adlra   :1;
                 uint8_t    refs    :2;
             };
         };
         enum {ps2 = 1, ps4 = 2, ps8 = 3, ps16 = 4, ps32 = 5, ps64 = 6, ps128 = 7,ps_none };
         enum {ref_aref = 0, ref_avcc = 1, ref_internal2_56 = 3};
         enum
         {
             __recommendedPrescaler = Controller_Configuration::controllerClk/200000UL,
             recommendedPrescalar = __recommendedPrescaler > 64? (ps128) : __recommendedPrescaler > 32? (ps64) :__recommendedPrescaler > (ps32)? 5:__recommendedPrescaler > 8? (ps16):__recommendedPrescaler > 4? (ps8):__recommendedPrescaler > 2? (ps4):(ps2)
         };

         // a way to encapsulate interrupt symbol to use in device specific structure
         // mainly for internal use, syntax not nice at all
         template<class T, void (T::*Fxn)()>
             static void setADCInterrupt(T & obj)
             {
                 redirectISRM(SIG_ADC, Fxn, obj);
             }


    }__attribute__((packed));
    /*@} ADconv --------------------------------------------------------------- */
}   // end of namespace atmega1281

#if defined(__AVR_ATmega1281__)
using namespace atmega1281;
#endif

/*@}*/

