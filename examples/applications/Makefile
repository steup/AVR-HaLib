#------- USER_CONFIG -------------

BIN=./bin
BUILD=./build
SRC=.
INC=.
HALIB:=../..

# Additional Libraries, without -l
LIBS=

#Locations
INCLUDES=${INC}
LDPATHS=


#----------END USER_CONFIG --------

.PHONY: default test

default: help

include ${HALIB}/config.mk
include ${HALIB}/make/example.mk

test:
	@echo ${INCLUDES}
