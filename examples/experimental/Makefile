#------- USER_CONFIG -------------

# Additional Libraries, without -l
LIBS=

BIN=./bin
BUILD=./build
SRC=.
INC=.
HALIB:=../..

USE_INT_MANAGER=1

#CHIP=atmega128rfa1
#CLOCK=8000000
CFLAGS  += -Wall -Os
CXXFLAGS+= -Wall -Os -fno-keep-inline-functions -ffunction-sections -fshort-enums #-DLOGGING_DISABLE
LDFLAGS+=-Os -Wl,--gc-sections
#Locations
INCLUDES+=${INC}
LDPATHS+=

PROGRAMMER=jtag2
PORT=usb

#----------END USER_CONFIG --------

.PHONY: default

default: help


include ${HALIB}/config.mk
include ${HALIB}/make/example.mk
include ${HALIB}/make/debug.mk
