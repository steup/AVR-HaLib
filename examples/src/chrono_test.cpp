//#define BOOST_NO_UNICODE_LITERALS
//#define BOOST_NOEXCEPT
//#define BOOST_CHRONO_HEADER_ONLY

#include <boost/chrono/duration.hpp>
#include <avr/io.h>

template<typename StorageType, typename granularity>
using TimeDuration = boost::chrono::duration<StorageType, granularity>;


typedef TimeDuration<int64_t, boost::ratio<1,16000000> > CpuTicks;
typedef TimeDuration<int64_t, boost::micro > MicroSeconds;

int main()
{
    CpuTicks d;
    d = MicroSeconds(1000);
    while(d.count()>0)
    {
        d-=CpuTicks(1);
        PINA|=1;
    }

    return 0;
}
