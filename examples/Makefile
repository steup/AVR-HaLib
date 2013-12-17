#------- USER_CONFIG -------------

HALIB_DIR        := ..

CFLAGS           += -Wall -Os
CXXFLAGS         += -Wall -Os -fno-keep-inline-functions -ffunction-sections -fshort-enums #-DLOGGING_DISABLE
LDFLAGS          += -Os -Wl,--gc-sections

#PLATFORM         := ${HALIB_DIR}/platform/deRCB128RFA1
PLATFORM         := ${HALIB_DIR}/platform/robbyboard2
#PLATFORM         := ${HALIB_DIR}/platform/icstick

PROGRAMMER       := avr911
PORT             := /dev/ttyUSB0
PM_SRC_DIRS      += ./portmaps

#----------END USER_CONFIG --------

include ${HALIB_DIR}/make/singleObjectApp.mk
include ${HALIB_DIR}/make/clean.mk
