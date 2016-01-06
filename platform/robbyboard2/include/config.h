#pragma once

#include <avr-halib/config/frequency.h>
#include <avr-halib/logging/loggingDevice.h>
#include <avr-halib/logging/uartLogging.h>
#include <avr-halib/common/sleep.h>
#include <boost/mpl/list.hpp>

namespace platform {
namespace robbyboard2 {

using avr_halib::logging::devices::Uart;

typedef avr_halib::config::Frequency< F_CPU > CPUClock;
typedef avr_halib::config::Frequency< 32768 > RTCClock;

struct LoggingConfig : public Uart::DefaultConfig
{
		typedef avr_halib::regmaps::local::Uart1 Timer;
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
		typedef avr_halib::regmaps::local::Timer2 Timer;
		typedef RTCClock TimerFrequency;
};

struct Timer0BaseConfig
{
		typedef avr_halib::regmaps::local::Timer0 Timer;
		typedef CPUClock TimerFrequency;
};

struct Timer1BaseConfig
{
		typedef avr_halib::regmaps::local::Timer1 Timer;
		typedef CPUClock TimerFrequency;
};

typedef AsyncTimerBaseConfig Timer2BaseConfig;

struct Timer3BaseConfig
{
		typedef avr_halib::regmaps::local::Timer3 Timer;
		typedef CPUClock TimerFrequency;
};

}}

#include <robbyboard2_portmap.h>
