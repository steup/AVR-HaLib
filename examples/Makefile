ifneq (${NOPMGEN},)
	PMGEN:=cat
else
	PMGEN:=${HALIB}/tools/portmapgen/avr-halib-pmg
endif

ifeq (${VERBOSE},)
	CC:=@${CC}
	CXX:=@${CXX}
	LD:=@${LD}
	OBJCP:=@${OBJCP}
	OBJDUMP:=@${OBJDUMP}
	SIZE:=@${SIZE}
	PMGEN:=@${PMGEN}
endif

CFLAGS+= -mmcu=${CHIP} -fno-strict-aliasing -fno-exceptions
CXXFLAGS:=${CFLAGS} ${CXXFLAGS} -fno-rtti -fno-threadsafe-statics
LDFLAGS+=-mmcu=${CHIP}
LIBS+=avr-halib-${CHIP}
OBJCPFLAGS= -j .data -j .text

BIN_KEEPER=${BIN}/.keep
BUILD_KEEPER=${BUILD}/.keep

INCLUDES:=$(addprefix -I,. ${INC} ${HALIB}/include ${INCLUDES})
LDPATHS:=$(addprefix -L,${LDPATHS} ${HALIB}/build)
LIBS:=$(addprefix -l,${LIBS})
EXAMPLES:=$(notdir $(basename $(shell find ${SRC} -maxdepth 1 -name "*.cpp" )))
DEPS:=$(addprefix ${BUILD}/, $(addsuffix .d, ${EXAMPLES}))
GARBAGE+=${BIN} ${BUILD} $(filter %~ %.sw? %.dump %.tag, $(shell find .))

.PHONY: default all clean %.program %.size ${EXAMPLES}
.PRECIOUS: ${BUILD}/%.elf ${BUILD}/%_pm.cpp

default:
	@echo "current examples: "
	@echo all ${EXAMPLES}

all: $(addprefix ${BIN}/, $(addsuffix .hex, ${EXAMPLES}))
	

${BUILD_KEEPER}:
	@mkdir ${BUILD}
	@touch ${BUILD_KEEPER}

${BIN_KEEPER}:
	@mkdir ${BIN}
	@touch ${BIN_KEEPER}

${BUILD}/%.d: ${SRC}/%.cpp ${BUILD_KEEPER}
	${CXX} -MM ${CFLAGS} $< -MF $@.temp ${INCLUDES} 2> /dev/null
	@echo -n "${BUILD}/" > $@
	@cat $@.temp >> $@
	@rm $@.temp

${BUILD}/%.elf: ${BUILD}/%.o ${BUILD_KEEPER}
	@echo "(LD)     $(notdir $<) -> $(notdir $@)"
	${CXX} ${LDFLAGS} $< -o $@ ${LDPATHS} ${LIBS}

${BUILD}/%.o: ${BUILD}/%_pm.cpp ${BUILD}/%.d ${BUILD_KEEPER}
	@echo "(CXX)    $(notdir $<) -> $(notdir $@)"
	${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%_pm.cpp: ${SRC}/%.cpp
	@echo "(PMGEN)  $(notdir $<) -> $(notdir $@)"
	${PMGEN} $< > $@

%.program: ${BIN}/%.hex
	@echo "(FLASH)  $(notdir $<) -> ${PORT} -> ${CHIP}"
	${FLASHER} ${FLASHOPTS} -P ${PORT} -p ${CHIP} -c ${PROGRAMMER} -U f:w:$<:i

%.size:	${BUILD}/%.elf
	${SIZE} $<

%.dump: ${BUILD}/%.elf
	@echo "(OBJDMP) $(notdir $<) -> $@"
	${OBJDUMP} -Cxd $< > $@

${BIN}/%.hex: ${BUILD}/%.elf ${BIN_KEEPER}
	@echo "(OBJCP)  $(notdir $<) -> $(notdir $@)"
	${OBJCP} ${OBJCPFLAGS} -O ihex $< $@

${EXAMPLES}: %: ${BIN}/%.hex

clean:
	@echo "(CLEAN)"
	@rm -rf ${GARBAGE}

-include ${DEPS}
