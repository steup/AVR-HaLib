#pragma once

#include <avr-halib/config/frequency.h>
#include <avr-halib/logging/loggingDevice.h>
#include <avr-halib/logging/uartLogging.h>
#include <avr-halib/common/sleep.h>
#include <boost/mpl/list.hpp>

namespace platform {
namespace icstick
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

    #include <icStick_portmap.h>
}
}
