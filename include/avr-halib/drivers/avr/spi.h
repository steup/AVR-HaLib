#pragma once

#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/config/spi.h>
#include <avr-halib/common/delegate.h>
#include <avr-halib/common/singleton.h>
#include <boost/mpl/if.hpp>
#include <boost/mpl/vector.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    using avr_halib::interrupts::interrupt_manager::Binding;
    using avr_halib::interrupts::interrupt_manager::Slot;

    /** \brief Serial Peripheral Interface Driver
     *
     * This driver uses the specific SPI hardware of different AVR chips to
     * enable SPI-based communication with other ICs. It currently only
     * supports Master-Mode, which is the initiator and clock generator of
     * every SPI communication.
     */
    struct Spi : public config::Spi
    {
        /** \brief Default Configuration */
        struct DefaultConfig
        {
            /** \brief TODO \todo */
            static const bool              useInterrupt = false;
            /** \brief TODO \todo */
            static const BitOrderType      bitOrder     = BitOrders::mostFirst;
            /** \brief TODO \todo */
            static const ClockPolarityType polarity     = ClockPolarities::idleOnHigh;
            /** \brief TODO \todo */
            static const SampleEdgeType    sampleEdge   = SampleEdges::leading;
            /** \brief TODO \todo */
            static const PrescalerType     prescaler    = 4;
            /** \brief TODO \todo */
            static const ModeType          mode         = Modes::master;
            /** \brief TODO \todo */

            /** \brief TODO \todo */
            typedef avr_halib::regmaps::local::Spi RegMap;
        };

        /** \brief Configuration meta function
         * \tparam Config Configuration description to use, defaults to \see DefaultConfig
         */
        template <typename Config = DefaultConfig>
        class configure
        {
            /** \brief Configured Driver */
            struct Core
            {
                protected:
                    /** \brief TODO \todo */
                    typedef typename Config::RegMap RegMap;
                    /** \brief TODO \todo */
                    bool busy;

                public:

                /** Construct driver and initialize hardware */
                Core()
                {
                    reset();
                }

                /** Reset and initialize hardware */
                void reset()
                {
                    typedef typename RegMap::template ClockConfig   < Config::prescaler,
                                                                      Config::polarity,
                                                                      Config::sampleEdge > Clock;
                    typedef typename RegMap::template BitOrderConfig< Config::bitOrder   > BitOrder;
                    typedef typename RegMap::template ModeConfig    < Config::mode       > Mode;


                    UseRegMap(rm, RegMap);
                    if(Config::mode==Modes::master)
                    {
                        rm.mosi.ddr  = true;
                        rm.miso.ddr  = false;
                        rm.miso.port = true;
                        rm.sck.ddr   = true;
                    }
                    else
                    {
                        rm.miso.ddr  = true;
                        rm.mosi.ddr  = false;
                        rm.mosi.port = true;
                        rm.ss.ddr    = false;
                        rm.ss.port   = true;
                        rm.sck.ddr   = false;
                        rm.sck.port  = true;
                    }

                    SyncRegMap(rm);

                    rm.mstr  = Mode::mstr;
                    rm.spe   = true;
                    rm.spie  = false;
                    rm.dord  = BitOrder::dord;
                    rm.cpol  = Clock::cpol;
                    rm.cpha  = Clock::cpha;;
                    rm.spr   = Clock::spr;
                    rm.spi2x = Clock::spi2x;

                    // Reset Flags

                    SyncRegMap(rm);

                    rm.spie  = Config::useInterrupt;
                    SyncRegMap(rm);

                    busy=false;
                }

                /** \brief Transmit one byte to the currently active slave
                 * \param c the byte to be transmitted
                 */
                bool put(const uint8_t c)
                {
                    UseRegMap(rm, RegMap);
                    rm.spdr = c;
                    SyncRegMap(rm);
                    busy = true;
                    return true;
                }

                /** \brief Test for ongoing operations
                 * \return true if none, false otherwise
                 */
                bool ready()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    if(busy)
                        if(rm.spif)
                        {
                            busy=false;
                            return true;
                        }
                        else
                            return false;
                    else
                        return true;
                }

                /** \brief Receive one byte from the currently active slave
                 * \param c Reference to be filled with received data
                 */
                bool get(uint8_t& c)
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    if(Config::mode==Modes::slave && !rm.spif)
                        return false;
                    c = rm.spdr;
                    return true;
                }
            };

            /** \brief Interrupt-Feature extension of core driver */
            struct InterruptExtension : public Core
            {
                public:
                    /** \brief TODO \todo */
                    typedef avr_halib::common::Delegate<void> CallbackType;
                    /** \brief TODO \todo */
                    typedef avr_halib::common::Singleton< InterruptExtension > Singleton;
                    /** \brief TODO \todo */
                    avr_halib::common::Delegate<void> callback;

                private:

                    /** \brief TODO \todo */
                    static void operationComplete()
                    {
                        Singleton& base = Singleton::getInstance();
                        base.busy=false;
                        base.callback();
                    }

                    /** \brief TODO \todo */
                    typedef Slot< Config::RegMap::InterruptMap::operationComplete,
                                           Binding::FixedPlainFunction > IntSlot;

                    /** \brief TODO \todo */
                    typedef typename IntSlot::template Bind< &InterruptExtension::operationComplete > BoundInt;

                public:
                    /** \brief TODO \todo */
                    typedef typename boost::mpl::vector< BoundInt >::type InterruptSlotList;
            };

            /** \brief TODO \todo */
            struct CoreProxy
            {
                private:
                    /** \brief TODO \todo */
                    typedef typename avr_halib::common::Singleton<Core>::type Base;

                public:
                    /** \brief TODO \todo */
                    typedef Config config;
                    /** \brief TODO \todo */
                    typedef typename boost::mpl::vector<>::type InterruptSlotList;

                    /** \brief TODO \todo */
                    void reset()
                    {
                        Base::getInstance().reset();
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    bool put(const uint8_t value)
                    {
                        return Base::getInstance().put(value);
                    }

                    /** \brief TODO \todo */
                    bool ready()
                    {
                        return Base::getInstance().ready();
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    bool get(uint8_t& value)
                    {
                        return Base::getInstance().get(value);
                    }
            };

            /** \brief TODO \todo */
            struct InterruptProxy
            {
                private:
                    /** \brief TODO \todo */
                    typedef typename InterruptExtension::Singleton Base;

                public:
                    /** \brief TODO \todo */
                    typedef Config config;
                    /** \brief TODO \todo */
                    typedef typename Base::CallbackType CallbackType;
                    /** \brief TODO \todo */
                    typedef typename Base::InterruptSlotList InterruptSlotList;

                    /** \brief TODO \todo */
                    void reset()
                    {
                        Base::getInstance().reset();
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    bool put(const uint8_t value)
                    {
                        return Base::getInstance().put(value);
                    }

                    /** \brief TODO \todo */
                    bool ready()
                    {
                        return Base::getInstance().ready();
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    bool get(uint8_t& value)
                    {
                        return Base::getInstance().get(value);
                    }

                    /** \brief TODO \todo */
                    const CallbackType& getCallback() const
                    {
                        return Base::getInstance().callback;
                    }

                    /** \brief TODO \todo
                     *
                     * \param cb TODO \todo
                     */
                    void setCallback(const CallbackType& cb)
                    {
                        Base::getInstance().callback=cb;
                    }
            };

            public:
                /** \brief Interrupt-Feature Selector */
                typedef typename boost::mpl::if_c< Config::useInterrupt,
                                                   InterruptProxy,
                                                   CoreProxy >::type type;
        };
    };
}
}
}
