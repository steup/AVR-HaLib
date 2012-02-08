#pragma once

#include <avr-halib/common/frequency.h>

typedef avr_halib::config::Frequency< 32768 > RTCClock;

#include <avr-halib/ext/loggingDevice.h>
#include <avr-halib/ext/uartLogging.h>

typedef avr_halib::logging::devices::Uart::configure<>::type LoggingDevice;

setLoggingDevice( LoggingDevice );

#include <avr-halib/ext/logging.h>

using avr_halib::logging::log;

#include <robbyboard_portmap.h>
#include <avr-halib/regmaps/local.h>
#include <avr-halib/ext/sensorPowerControl.h>

typedef avr_halib::drivers::robby::SensorPowerControl< int, false > SensorPower;

#include <avr-halib/avr/sleep.h>
#include <boost/mpl/list.hpp>

struct MorpheusConfig : public avr_halib::power::Morpheus::DefaultConfig
{
    typedef boost::mpl::list< LoggingDevice::SleepSync >::type SyncList;
};

typedef avr_halib::power::Morpheus::configure< MorpheusConfig >::type Morpheus;
