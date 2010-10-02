#------- USER_CONFIG -------------

# Additional Libraries, without -l
LIBS=

#Locations
INCLUDES=
LDPATHS=

BIN=./bin
BUILD=./build
SRC=.
INC=.
HALIB:=../..

#----------END USER_CONFIG --------

.PHONY: default test

default: help

include ${HALIB}/config.mk
include ${HALIB}/rules/general.mk
include ${HALIB}/rules/example.mk

test:
	@echo ${INCLUDES}
