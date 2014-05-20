#pragma once

#include <avr-halib/share/interruptLock.h>

namespace avr_halib
{
namespace locking
{
	struct HardwareMutex
	{
        struct DefaultConfig
        {
            typedef typename regmaps::local::HWMutex0 RegMap;
        };

        template<typename Config=DefaultConfig>
        struct configure
        {
            struct type
            {
                private:
                    typedef typename Config::RegMap RegMap;

                public:
                    static bool aquire()
                    {
                        UseRegMap(rm, RegMap);
                        SyncRegMap(rm);

                        GlobalIntLock lock;

                        if(rm.value)
                            return false;

                        rm.value=true;
                        SyncRegMap(rm);

                        return true;
                    }

                    static void release()
                    {
                        UseRegMap(rm, RegMap);
                        rm.value=false;
                        SyncRegMap(rm);
                    }
            };
        };
	};
}
}
