#------- USER_CONFIG -------------

# Tools used
CC=avr-gcc
CXX=avr-g++
LD=avr-ld
OBJCP=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size

# Config parameters
CHIP=at90can128
CFLAGS=-Wall -Os
CXXFLAGS=
LDFLAGS=

# Additional Libraries, without -l
LIBS=

#Locations
INCLUDES=${HOME}/software/include/avr
LDPATHS=${HOME}/software/lib/avr /usr/x86_64-pc-linux-gnu/avr/lib

# Config for flashing
FLASHER=avrdude
PROGRAMMER=avr911
FLASHOPTS=
PORT=/dev/ttyUSB0

#some paths, which the user might want to change
INC=
SRC=./
HALIB=../../include

#----------END USER_CONFIG --------

PMGEN:=${HALIB}/../tools/portmapgen/avr-halib-pmg

CFLAGS+= -mmcu=${CHIP} -fno-strict-aliasing -fno-rtti -fno-exceptions
CXXFLAGS:=${CFLAGS} ${CXXFLAGS}
LDFLAGS+=-mmcu=${CHIP}
LIBS+=avr-halib-${CHIP}
OBJCPFLAGS= -j .data -j .text

BIN=./bin
BUILD=./build
BIN_KEEPER=${BIN}/.keep
BUILD_KEEPER=${BUILD}/.keep

INCLUDES:=$(addprefix -I,. ${INC} ${HALIB} ${INCLUDES})
LDPATHS:=$(addprefix -L, ${LDPATHS})
LIBS:=$(addprefix -l, ${LIBS})
EXAMPLES:=$(notdir $(basename $(shell find ${SRC} -name "*.cpp")))
DEPS:=$(addprefix ${BUILD}/, $(addsuffix .d, ${EXAMPLES}))
GARBAGE=$(filter-out . %.cpp %.h %.svn %Makefile,$(shell find . -maxdepth 1))

.PHONY: default all clean install_% size_% ${EXAMPLES}
.PRECIOUS: ${BUILD}/%.elf

default:
	@echo "current examples: "
	@echo ${EXAMPLES}

all: $(addprefix ${BIN}/, $(addsuffix .hex, ${EXAMPLES}))
	

${BUILD_KEEPER}:
	@mkdir ${BUILD}
	@touch ${BUILD_KEEPER}

${BIN_KEEPER}:
	@mkdir ${BIN}
	@touch ${BIN_KEEPER}

${BUILD}/%.d: ${SRC}/%.cpp ${BUILD_KEEPER}
	@${CXX} -MM ${CFLAGS} $< -MF $@.temp ${INCLUDES}
	@echo -n "${BUILD}/" > $@
	@cat $@.temp >> $@
	@rm $@.temp

${BUILD}/%.elf: ${BUILD}/%.o ${BUILD_KEEPER}
	@echo "(LD)     $(notdir $<) -> $(notdir $@)"
	@${CXX} ${LDFLAGS} $< -o $@ ${LDPATHS} ${LIBS}

${BUILD}/%.o: ${BUILD}/%_pm.cpp ${BUILD}/%.d ${BUILD_KEEPER}
	@echo "(CXX)    $(notdir $<) -> $(notdir $@)"
	@${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%_pm.cpp: ${SRC}/%.cpp
	@echo "(PMGEN)  $(notdir $<) -> $(notdir $@)"
	@${PMGEN} $< > $@

install_%: ${BIN}/%.hex
	@echo "(FLASH)  $(notdir $<) -> ${PORT} -> ${CHIP}"
	@${FLASHER} ${FLASHOPTS} -P ${PORT} -p ${CHIP} -c ${PROGRAMMER} -U f:w:$<:i

size_%:	${BUILD}/%.elf
	@${SIZE} $<

dump_%: ${BUILD}/%.elf
	@echo "(OBJDMP) $(notdir $<) -> $(notdir $(basename $<)).dump"
	@${OBJDUMP} -Cxd $< > $(notdir $(basename $<)).dump

${BIN}/%.hex: ${BUILD}/%.elf ${BIN_KEEPER}
	@echo "(OBJCP)  $(notdir $<) -> $(notdir $@)"
	@${OBJCP} ${OBJCPFLAGS} -O ihex $< $@

${EXAMPLES}: %: ${BIN}/%.hex

clean:
	@echo "(CLEAN)"
	@rm -rf ${GARBAGE}

-include ${DEPS}
