#------- USER_CONFIG -------------

# Additional Libraries, without -l
LIBS=

BIN=./bin
BUILD=./build
SRC=.
INC=.
HALIB:=../..

CXXFLAGS= -Wall -Os -fno-keep-inline-functions -ffunction-sections -fshort-enums #-DLOGGING_DISABLE
LDFLAGS=-Os -Wl,--gc-sections #-Wl,--section-start=.text=0x1FC00
#Locations
INCLUDES=${INC}
LDPATHS=

#PROGRAMMER=jtag2
#PORT=usb

#----------END USER_CONFIG --------

.PHONY: default

default: help


include ${HALIB}/config.mk
LDFLAGS=-TInterruptManager/avr5.x
include ${HALIB}/make/example.mk
include ${HALIB}/make/debug.mk
