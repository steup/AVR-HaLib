#------- USER_CONFIG -------------

HALIB_DIR        := ../..

USE_INT_MANAGER  := 1


CFLAGS           += -Wall -Os
CXXFLAGS         += -Wall -Os -fno-keep-inline-functions -ffunction-sections -fshort-enums #-DLOGGING_DISABLE
LDFLAGS          +=-Os -Wl,--gc-sections

PROGRAMMER       := jtag2
PORT             := usb

#----------END USER_CONFIG --------

include ${HALIB_DIR}/config.mk
include ${HALIB_DIR}/make/singleObjectApps.mk
include ${HALIB_DIR}/make/clean.mk

