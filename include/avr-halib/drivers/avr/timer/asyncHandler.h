#pragma once

namespace avr_halib
{
namespace drivers
{
namespace avr
{
namespace timer
{
    template<typename RegMap, bool asyncEnabled>
    struct AsyncHandler;

    template<typename config>
    struct AsyncHandler<config, true>
    {
        private:
            typedef typename config::RegMap RegMap;

        protected:
            AsyncHandler()
            {
                UseRegMap(rm, RegMap);
                rm.as=!config::asyncExtClock;
                rm.extclk=config::asyncExtClock;
            }

            void sync() const
            {
                UseRegMap(rm, RegMap);
                while(rm.tcrub || rm.tcnub)
                    SyncRegMap(rm);
            }
    };

    template<typename config>
    struct AsyncHandler<config, false>
    {
        protected:
            void sync(){}
    };
}
}
}
}
