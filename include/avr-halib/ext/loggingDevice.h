#pragma once

#define setLoggingDevice(device) 							\
	namespace avr_halib 									\
	{														\
	namespace logging 										\
	{														\
	namespace config										\
	{														\
		typedef device LoggingDevice;                       \
	}														\
	}														\
	}														
