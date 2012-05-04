THIS_DIR         := $(dir $(abspath $(lastword ${MAKEFILE_LIST})))
TARGET           := atmega1281
CLOCK            := 8000000
INCLUDES         += ${THIS_DIR}/include
PM_SRC_DIRS      += ${THIS_DIR}/portmap
PROGRAMMER       := avr911
PORT             := /dev/ttyUSB0
