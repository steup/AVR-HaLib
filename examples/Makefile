#------- USER_CONFIG -------------

HALIB_DIR        := ..

CFLAGS           += -Wall -Os -g
CXXFLAGS         += -Wall -Os -g
CXXFLAGS         += -fno-keep-inline-functions -fshort-enums -ffunction-sections
#CXXFLAGS         += -DLOGGING_DISABLE
LDFLAGS          += -O1

PLATFORM         := ${HALIB_DIR}/platform/robbyboard2

PM_SRC_DIRS      += ./portmaps

#----------END USER_CONFIG --------

include ${HALIB_DIR}/make/singleObjectApp.mk
include ${HALIB_DIR}/make/clean.mk
