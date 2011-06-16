PMGENBIN:=${HALIB}/tools/portmapgen/avr-halib-pmg
PMGEN=${PMGENBIN}

ifeq (${VERBOSE},)
	CC:=@${CC}
	CXX:=@${CXX}
	AS:=@${AS}
	LD:=@${LD}
	OBJCP:=@${OBJCP}
	OBJDUMP:=@${OBJDUMP}
	SIZE:=@${SIZE}
	AR:=@${AR}
	PMGEN:=@${PMGEN}
endif

GENDIRS=${BIN} ${LIB} ${BUILD}

SOURCES=$(wildcard ${SRC}/*.cpp ${SRC}/*.cc ${SRC}/*.c ${SRC}/*.S)

LDPATHS:=$(addprefix -L,${LDPATHS})

LIBS:=$(addprefix -l,${LIBS})
INCLUDES:=$(addprefix -I,${INCLUDES})

OBJECTS=$(foreach OBJ, ${SOURCES}, ${BUILD}/$(notdir $(basename ${OBJ})).o)

DEPS=$(wildcard ${BUILD}/*.d)

GARBAGE+=${GENDIRS}

.PHONY: %.size %.program
.PRECIOUS: ${BUILD}/%.d

${GENDIRS}: %:
	@mkdir -p $@

${BUILD}/%.d: ${SRC}/%.cpp |${BUILD}
	${CXX} -MT $@ -MG -MM ${CXXFLAGS} $< ${INCLUDES} -MF $@ 

${BUILD}/%.d: ${SRC}/%.cc |${BUILD}
	${CXX} -MT $@ -MG -MM ${CXXFLAGS} $< ${INCLUDES} -MF $@ 

${BUILD}/%.d: ${SRC}/%.c |${BUILD}
	${CC} -MT $@ -MG -MM ${CFLAGS} $< ${INCLUDES} -MF $@ 

${BUILD}/%.d: ${SRC}/%.S |${BUILD}
	${AS} -MT $@ -MG -MM ${ASMFLAGS} $<  ${INCLUDES} -MF $@

${BUILD}/%.o: ${SRC}/%.cpp ${BUILD}/%.d ${ADDITIONAL_DEPS} |${BUILD}
	@echo "(CXX   ) $(notdir $<) -> $(notdir $@)"
	${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: ${SRC}/%.cc ${BUILD}/%.d ${ADDITIONAL_DEPS} |${BUILD}
	@echo "(CXX   ) $(notdir $<) -> $(notdir $@)"
	${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: ${SRC}/%.c ${BUILD}/%.d ${ADDITIONAL_DEPS} |${BUILD}
	@echo "(CC    ) $(notdir $<) -> $(notdir $@)"
	${CC} -c ${CFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: ${SRC}/%.S ${BUILD}/%.d ${ADDITIONAL_DEPS} |${BUILD}
	@echo "(AS    ) $(notdir $<) -> $(notdir $@)"
	${AS} -c ${ASMFLAGS} $< -o $@ ${INCLUDES}

${INC}/%_portmap.h: ${INC}/%.portmap | ${PMGENBIN}
	@echo "(PMGEN ) $(notdir $<) -> $(notdir $@)"
	${PMGEN} $< > $@

%.size:	${BIN}/%.elf
	${SIZE} $<

%.dump: ${BIN}/%.elf
	@echo "(OBJDMP) $(notdir $<) -> $@"
	${OBJDUMP} -Cxd $< > $@

${BIN}/%.hex: ${BIN}/%.elf |${BIN}
	@echo "(OBJCP ) $(notdir $<) -> $(notdir $@)"
	${OBJCP} ${OBJCPFLAGS} -O ihex $< $@

${LIB}/lib${LIBNAME}.a: ${OBJECTS} | ${LIB} 
	@echo "(AR    ) $@ <- $(notdir ${OBJECTS})"
	$(AR) ${ARFLAGS} $@ ${OBJECTS}

%.program: ${BIN}/%.hex
	@echo "(FLASH)  $(notdir $<) -> ${PORT} -> ${CHIP}"
	${FLASHER} ${FLASHOPTS} -P ${PORT} -p ${CHIP} -c ${PROGRAMMER} -U f:w:$<:i

-include ${DEPS}
