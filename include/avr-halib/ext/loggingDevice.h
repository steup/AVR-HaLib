#pragma once

#define setLoggingDevice(device) 							\
	namespace avr_halib 									\
	{														\
	namespace logExt										\
	{														\
	namespace config										\
	{														\
		template<typename config>							\
		struct LoggingDevice : public device<config>{};		\
	}														\
	}														\
	}

#define setLoggingConfig(logConf) 							\
	namespace avr_halib										\
	{														\
	namespace logExt										\
	{														\
	namespace config										\
	{														\
		typedef logConf LoggingConfig;						\
	}														\
	}														\
	}														
