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

HALIB        := AVR

ifdef USE_INT_MANAGER
	CFLAGS   :=${CFLAGS} -DUSE_INT_MANAGER=1
	CXXFLAGS :=${CXXFLAGS} -DUSE_INT_MANAGER=1
	LDFLAGS  :=${LDFLAGS} -Wl,-T${LDSCRIPTS}/avr5.x
else
	CFLAGS   :=${CFLAGS} -DUSE_INT_MANAGER=0
	CXXFLAGS :=${CXXFLAGS} -DUSE_INT_MANAGER=0
endif

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
LDFLAGS      += -mmcu=${TARGET}
LIBS         += avr-halib
LDPATHS      += ${HALIB_DIR}/lib/${TARGET}
OBJCPFLAGS   += -j .data -j .text
ARFLAGS      := rus

INCLUDES     += ${INC} \
				${HALIB_DIR}/include

ADDITIONAL_DEPS += externals
