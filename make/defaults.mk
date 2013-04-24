include $(dir $(abspath $(lastword ${MAKEFILE_LIST})))/../config.mk

TOOL_PREFIX  ?= avr-

CC           := gcc
ASM          ?= gcc
CXX          ?= g++
LD           ?= ld
AR           ?= ar
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
AR           := @${TOOL_PREFIX}${AR}
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

AVR_ASMFLAGS := -mmcu=${TARGET} \
				-DF_CPU=${CLOCK}ULL

AVR_CXXFLAGS := -mmcu=${TARGET} \
				-DF_CPU=${CLOCK}ULL \
				-fno-strict-aliasing \
			    -fno-exceptions \
			    -fno-rtti \
				-fno-threadsafe-statics

AVR_LDFLAGS  := -mmcu=${TARGET} \
				-T${LDSCRIPTS}/${TARGET}.x \
				-Wl,--gc-sections

LIBS         += avr-halib
LDPATHS      += ${HALIB_DIR}/lib/${TARGET}
ARFLAGS      := rus

INCLUDES     := ${INC} \
				${INCLUDES} \
				${HALIB_DIR}/include

ADDITIONAL_DEPS += externals
