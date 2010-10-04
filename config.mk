#------- USER_CONFIG -------------

# Tools used
CC=avr-gcc
CXX=avr-g++
AS=avr-gcc
AR=avr-ar
LD=avr-ld
OBJCP=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size

# Chip dependent configuration

#Chip type
CHIP=at90can128

# Clock frequency in Hz
CLOCK=16000000

# Compiler linker etc. flags
CFLAGS+=-Wall -Os -g
CXXFLAGS+=
ASMFLAGS+=
LDFLAGS+=
ARFLAGS+=

# Config for flashing
FLASHER=avrdude
PROGRAMMER=avr911
FLASHOPTS+=
PORT=/dev/ttyUSB0

# Additional includes
INCLUDE+=

# Additional library search pathes
LDPATHS+=

# Additional libraries
LIBS+=

#Mandatory dependancies

#Boost include path, if non default path(/usr/include/boost)
#BOOST_DIR=

#C++ Logging framework include path
LOGGING_DIR=/home/mschulze/SRC/logging-cpp/include

#----------END USER_CONFIG --------
