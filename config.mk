#------- USER_CONFIG -------------

#TOOL_PREFIX ?= avr-

# Chip dependent configuration

#Target MCU
TARGET       ?= at90can128

# Clock frequency in Hz
CLOCK        ?= 16000000

# Compiler linker etc. flags
CFLAGS       +=
CXXFLAGS     +=
ASMFLAGS     +=
LDFLAGS      +=
ARFLAGS      +=

# Config for flashing
#FLASHER      ?=avrdude
#PROGRAMMER   ?=avr911
#FLASHOPTS    +=
#PORT         ?=/dev/ttyUSB0

# Additional includes
INCLUDE      +=

# Additional library search pathes
LDPATHS      +=

# Additional libraries
LIBS         +=

#Mandatory dependancies

#Boost include path, if non default path(/usr/include/boost)
#BOOST_DIR   :=

#C++ Logging framework include path
LOGGING_DIR  :=

#----------END USER_CONFIG --------
