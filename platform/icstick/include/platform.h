#pragma once

#include "config.h"

namespace platform
{
    using namespace icstick;
}

setLoggingDevice( platform::LoggingDevice );

#include <avr-halib/logging/logging.h>

using avr_halib::logging::log;
using platform::Morpheus;
