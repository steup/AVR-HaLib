#------- USER_CONFIG -------------

HALIB_DIR        := ../..

USE_INT_MANAGER  := 1


CFLAGS           += -Wall -Os -g
CXXFLAGS         += -Wall -Os -g -fno-keep-inline-functions -ffunction-sections -fshort-enums -DLOGGING_DISABLE #-fno-inline
LDFLAGS          += -Os -Wl,--gc-sections -Wl,--section-start=.text=0x1e000

PROGRAMMER       := jtag2
PORT             := usb

#----------END USER_CONFIG --------

include ${HALIB_DIR}/config.mk
include ${HALIB_DIR}/make/singleObjectApps.mk
include ${HALIB_DIR}/make/clean.mk

