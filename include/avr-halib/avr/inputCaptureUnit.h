#pragma once

#include <avr-halib/config/interrupt.h>
#include <avr-halib/common/staticDelegate.h>

namespace avr_halib{
namespace driver
{
    struct InputCaptureUnit
    {
        struct Edges
        {
            enum EdgeType
            {
                falling,
                rising
            };
        };
        typedef Edges::EdgeType EdgeType;

        struct DefaultConfig : public config::Interrupt
        {
            static const bool          noiseCancel = false;
            static const EdgeType      edge        = Edges::falling;
            static const InterruptType intType     = InterruptTypes::dynamic;
            typedef regmaps::local::Timer1 RegMap;
        };

        template<typename Config = DefaultConfig>
        class configure
        {
            typedef ::Interrupt::Binding Binding;
            typedef typename Config::RegMap::InterruptMap IntMap;
            typedef config::Interrupt::InterruptTypes IntTypes;

            struct Core
            {
                Core()
                {
                    UseRegMap(rm, typename Config::RegMap);

                    rm.ices = Config::edge;
                    rm.icnc = Config::noiseCancel;
                    SyncRegMap(rm);
                    rm.icf  = true;
                    SyncRegMap(rm);
                }

                void enable()
                {
                    UseRegMap(rm, typename Config::RegMap);
                    rm.icie = true;
                    SyncRegMap(rm);
                }

                void disable()
                {
                    UseRegMap(rm, typename Config::RegMap);
                    rm.icie = false;
                    SyncRegMap(rm);
                }

                bool triggered() const
                {
                    UseRegMap(rm, typename Config::RegMap);
                    return rm.icf;
                }

                uint16_t value() const
                {
                    UseRegMap(rm, typename Config::RegMap);
                    return rm.icr;
                }
            };

            struct DynamicInterrupt : public Core
            {
                typedef Config config;

                typedef ::Interrupt::Slot< IntMap::capture, Binding::DynamicPlainFunction > IntSlot;

                typedef typename boost::mpl::vector< IntSlot >::type InterruptSlotList;
                typedef typename IntSlot::CallbackType CallbackType;

                DynamicInterrupt()
                {
                    IntSlot::reset();
                }

                void setCallback(const CallbackType& cb)
                {
                    IntSlot::setCallback(cb);
                }
            };

            struct StaticInterrupt : public Core
            {
                typedef Config config;
                typedef avr_halib::object::StaticDelegate<void> StaticCallbackType;


                typedef ::Interrupt::Slot< IntMap::capture, Binding::FixedPlainFunction > IntSlot;

                template<typename StaticCallback>
                struct setCallback
                {
                    typedef typename IntSlot::template setCallback< StaticCallback > BoundSlot;
                    typedef typename boost::mpl::vector< BoundSlot >::type InterruptSlotList;
                };
            };

            public:
                typedef typename boost::mpl::if_c< Config::intType == IntTypes::dynamic, 
                                                   DynamicInterrupt,
                                                   StaticInterrupt
                                                   >::type type;
        };
    };
}
}
