#------- USER_CONFIG -------------

AVR_HALIB_DIR  := $(dir $(abspath $(lastword ${MAKEFILE_LIST})))

#TOOL_PREFIX ?= avr-

# Compiler linker etc. flags
CFLAGS       +=
CXXFLAGS     +=
ASMFLAGS     +=
LDFLAGS      +=
ARFLAGS      +=

# Additional includes
INCLUDE      +=

# Additional library search pathes
LDPATHS      +=

# Additional libraries
LIBS         +=

#Mandatory dependancies

#Boost include path, if non default path(/usr/include/boost)
BOOST_DIR   := /usr/local/include/boost

#C++ Logging framework include path
LOGGING_DIR  := ${AVR_HALIB_DIR}/externals/logging-cpp


