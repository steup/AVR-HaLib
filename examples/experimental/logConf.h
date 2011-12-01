#pragma once

#include <avr-halib/share/freq.h>
#include <avr-halib/ext/loggingDevice.h>
#include <avr-halib/avr/sleep.h>
#include <boost/mpl/list.hpp>

using avr_halib::config::Frequency;

typedef Frequency<F_CPU> CPUClock;

#include <avr-halib/ext/uartLogging.h>
#include <avr-halib/avr/uartSync.h>
#include <avr-halib/avr/regmaps.h>

typedef Uart1<CPUClock,19200> logConf;

setLoggingConfig(logConf);

setLoggingDevice(avr_halib::logExt::devices::Uart);

typedef avr_halib::power::UartSleepSynchronizer<logConf> LogSync;

typedef boost::mpl::list<LogSync>::type MorpheusSyncList;

/*
#include <avr-halib/ext/lcdLogging.h>
#include <avr-halib/portmaps/lcd_hd44780.h>

setLoggingConfig(LcdHd44780Board);

setLoggingDevice(avr_halib::logging::devices::LCD);

typedef boost::mpl::list<>::type MorpheusSyncList;
*/

#include <avr-halib/ext/logging.h>

using avr_halib::logExt::log;

