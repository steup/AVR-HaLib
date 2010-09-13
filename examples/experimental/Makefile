#------- USER_CONFIG -------------

# Tools used
CC=avr-gcc
CXX=avr-g++
LD=avr-ld
OBJCP=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size

# Config parameters
CHIP=at90can128
CLOCK=16000000
CFLAGS=-Wall -Os -g -DF_CPU=${CLOCK}ULL -D__NO_STL__ -DBOOST_NO_STDLIB_CONFIG
CXXFLAGS=
LDFLAGS=-T ldscripts/avr51.x

# Additional Libraries, without -l
LIBS=

# Config for flashing
FLASHER=avrdude
PROGRAMMER=avr911
FLASHOPTS=
PORT=/dev/ttyUSB0

#some paths, which the user might want to change
INC=.
SRC=.
BUILD=./build
BIN=./bin
HALIB=../..

#Locations
INCLUDES=../../experimental/include \
		 ${HOME}/software/include/avr \
		 ${HOME}/software/include/avr/boost/compatibility/cpp_c_headers
LDPATHS=/usr/x86_64-pc-linux-gnu/avr/lib

#----------END USER_CONFIG --------

include ${HALIB}/examples/Makefile
