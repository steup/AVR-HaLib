#------- USER_CONFIG -------------

# Additional Libraries, without -l
LIBS=

CXXFLAGS+=-fno-keep-inline-functions -ffunction-sections -fshort-enums #-DLOGGING_DISABLE
LDFLAGS=-Wl,--gc-sections #-Wl,--section-start=.text=0x1FC00
#Locations
INCLUDES=
LDPATHS=

BIN=./bin
BUILD=./build
SRC=.
INC=.
HALIB:=../..
#PROGRAMMER=jtag2
#PORT=usb

#----------END USER_CONFIG --------

.PHONY: default test

default: help

include ${HALIB}/config.mk
LDFLAGS=-TInterruptManager/avr5.x
include ${HALIB}/rules/general.mk
include ${HALIB}/rules/example.mk

test:
	@echo ${INCLUDES}
