#pragma once

#define setLoggingDevice(device) 							\
	namespace avr_halib 									\
	{														\
	namespace logging										\
	{														\
	namespace config										\
	{															\
		template<typename config>							\
		struct LoggingDevice : public device<config>{};		\
	}														\
	}														\
	}

#define setLoggingConfig(config) 							\
	typedef config LoggingConfig;
