#pragma once

#include <avr-halib/common/frequency.h>

typedef avr_halib::config::Frequency< 32768 > RTCClock;

#include <avr-halib/ext/loggingDevice.h>
#include <avr-halib/ext/uartLogging.h>

struct UartConfig : public avr_halib::logging::devices::Uart::DefaultConfig
{
    typedef avr_halib::regmaps::local::Uart1 RegMap;
};

typedef avr_halib::logging::devices::Uart::configure<UartConfig>::type LoggingDevice;

setLoggingDevice( LoggingDevice );

#include <avr-halib/ext/logging.h>

using avr_halib::logging::log;

#include <avr-halib/avr/sleep.h>
#include <boost/mpl/list.hpp>

struct MorpheusConfig : public avr_halib::power::Morpheus::DefaultConfig
{
    typedef boost::mpl::list< LoggingDevice::SleepSync >::type SyncList;
};

typedef avr_halib::power::Morpheus::configure< MorpheusConfig >::type Morpheus;

#include <deRCB128RFA1_portmap.h>
