include ${HALIB_DIR}/config.mk

TOOL_PREFIX  ?= avr-

CC           ?= gcc
ASM          ?= gcc
CXX          ?= g++
LD           ?= ld
SIZE         ?= size
OBJCPY       ?= objcopy
OBJDMP       ?= objdump
MKDIR        ?= mkdir -p
REMOVE       ?= rm -rf
LINK         ?= ln


CC           := @${TOOL_PREFIX}${CC}
ASM          := @${TOOL_PREFIX}${ASM}
CXX          := @${TOOL_PREFIX}${CXX}
LD           := @${TOOL_PREFIX}${LD}
SIZE         := @${TOOL_PREFIX}${SIZE}
OBJCPY       := @${TOOL_PREFIX}${OBJCPY}
OBJDMP       := @${TOOL_PREFIX}${OBJDMP}
MKDIR        := @${MKDIR}
REMOVE       := @${REMOVE}
LINK         := @${LINK}


SRC          ?= ./src
INC          ?= ./include
BIN          ?= ./bin
BUILD        ?= ./build
LDSCRIPTS    ?= ${HALIB_DIR}/ldscripts

HALIB        := AVR

include ${PLATFORM}/config.mk

AVR_CFLAGS   := -mmcu=${TARGET} \
				-DF_CPU=${CLOCK}ULL \
				-fno-strict-aliasing \
			    -fno-exceptions

AVR_CXXFLAGS := -mmcu=${TARGET} \
				-DF_CPU=${CLOCK}ULL \
				-fno-strict-aliasing \
			    -fno-exceptions \
			    -fno-rtti \
				-fno-threadsafe-statics

ASMFLAGS     += ${CFLAGS}
LDFLAGS      += -mmcu=${TARGET} -T${LDSCRIPTS}/${TARGET}.x
LIBS         += avr-halib
LDPATHS      += ${HALIB_DIR}/lib/${TARGET}
ARFLAGS      := rus

INCLUDES     += ${INC} \
				${HALIB_DIR}/include

ADDITIONAL_DEPS += externals
