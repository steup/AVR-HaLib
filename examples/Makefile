#------- USER_CONFIG -------------

HALIB_DIR        := ..

#USE_INT_MANAGER := 1

TARGET           :=atmega128rfa1
CLOCK            :=16000000
CFLAGS           += -Wall -Os
CXXFLAGS         += -Wall -Os -fno-keep-inline-functions -ffunction-sections -fshort-enums #-DLOGGING_DISABLE
LDFLAGS          +=-Os -Wl,--gc-sections

#----------END USER_CONFIG --------

include ${HALIB_DIR}/config.mk
include ${HALIB_DIR}/make/singleObjectApps.mk
include ${HALIB_DIR}/make/clean.mk

