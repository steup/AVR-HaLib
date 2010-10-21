#------- USER_CONFIG -------------

# Additional Libraries, without -l
LIBS=

#Locations
INCLUDES=
LDPATHS=

LDFLAGS=-Wl,-Tavr5.x,--gc-sections
CXXFLAGS+=-fno-keep-inline-functions -ffunction-sections

BIN=./bin
BUILD=./build
SRC=.
INC=.
HALIB:=../../..

#----------END USER_CONFIG --------

.PHONY: default test

default: help

include ${HALIB}/rules/loadconfig.mk
include ${HALIB}/rules/general.mk
include ${HALIB}/rules/example.mk

test:
	@echo ${INCLUDES}
