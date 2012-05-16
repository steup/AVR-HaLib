#pragma once

#include <avr-halib/common/frequency.h>
#include <avr-halib/ext/loggingDevice.h>
#include <avr-halib/ext/uartLogging.h>
#include <avr-halib/avr/sleep.h>
#include <boost/mpl/list.hpp>

namespace platform {
namespace deRCB128RFA1
{
    using avr_halib::logging::devices::Uart;

    typedef avr_halib::config::Frequency< F_CPU > CPUClock;
    typedef avr_halib::config::Frequency< 32768 > RTCClock;

    struct LoggingConfig : public Uart::DefaultConfig
    {
        typedef avr_halib::regmaps::local::Uart1 RegMap;
        static const Uart::BaudRateType baudRate = 19200;
    };

    typedef Uart::configure<LoggingConfig>::type LoggingDevice;

    struct MorpheusConfig : public avr_halib::power::Morpheus::DefaultConfig
    {
        typedef boost::mpl::list< LoggingDevice::SleepSync >::type SyncList;
    };

    typedef avr_halib::power::Morpheus::configure< MorpheusConfig >::type Morpheus;

    struct AsyncTimerBaseConfig
    {
        typedef avr_halib::regmaps::local::Timer2 RegMap;
        typedef RTCClock InputFrequency;
    };

    struct Timer0BaseConfig
    {
        typedef avr_halib::regmaps::local::Timer0 RegMap;
        typedef CPUClock InputFrequency;
    };    

    struct Timer1BaseConfig
    {
        typedef avr_halib::regmaps::local::Timer1 RegMap;
        typedef CPUClock InputFrequency;
    };    

    typedef AsyncTimerBaseConfig Timer2BaseConfig;

    struct Timer3BaseConfig
    {
        typedef avr_halib::regmaps::local::Timer3 RegMap;
        typedef CPUClock InputFrequency;
    };

    struct Timer4BaseConfig
    {
        typedef avr_halib::regmaps::local::Timer4 RegMap;
        typedef CPUClock InputFrequency;
    };

    struct Timer5BaseConfig
    {
        typedef avr_halib::regmaps::local::Timer5 RegMap;
        typedef CPUClock InputFrequency;
    };

    #include <deRCB128RFA1_portmap.h>
}
}
