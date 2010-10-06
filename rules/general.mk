PMGENBIN:=${HALIB}/tools/portmapgen/avr-halib-pmg
PMGEN=${PMGENBIN}

PORTMAPS=$(addsuffix .h, $(basename $(wildcard ${PORTMAPDIR}/*.portmap)))

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

CFLAGS+=-mmcu=${CHIP} -DF_CPU=${CLOCK}ULL -D__NO_STL__ -DBOOST_NO_STDLIB_CONFIG -fno-strict-aliasing -fno-exceptions
CXXFLAGS+=${CFLAGS} -fno-rtti -fno-threadsafe-statics
ASMFLAGS+=${CFLAGS}
LDFLAGS+=-mmcu=${CHIP}
LIBS+=avr-halib-${CHIP}
OBJCPFLAGS+= -j .data -j .text
ARFLAGS=rus

BOOST_DIR?=/usr/include/

INCLUDES += ${INC} \
		 	${HALIB}/include \
			${HALIB}/experimental/include \
		    ${BOOST_DIR} \
			${BOOST_DIR}/boost/compatibility/cpp_c_headers/ \
		    ${LOGGING_DIR}

GENDIRS=${BIN} ${LIB} ${BUILD}

SOURCES=$(wildcard ${SRC}/*.cpp ${SRC}/*.c ${SRC}/*.S)

LDPATHS:=$(addprefix -L,${LDPATHS} ${HALIB}/build)

LIBS:=$(addprefix -l,${LIBS})
INCLUDES:=$(addprefix -I,${INCLUDES})

OBJECTS=$(foreach OBJ, ${SOURCES}, ${BUILD}/$(notdir $(basename ${OBJ})).o)

DEPS=$(foreach DEP, ${SOURCES}, ${BUILD}/$(notdir $(basename ${DEP})).d)

GARBAGE+=

.PHONY: %.size

${GENDIRS}: %:
	@mkdir -p $@

${BUILD}/%.d: ${SRC}/%.cpp |${BUILD}
	${CXX} -MG -MM ${CXXFLAGS} $< -MF $@.temp ${INCLUDES}
	@echo -n "${BUILD}/" > $@
	@cat $@.temp >> $@
	@rm $@.temp

${BUILD}/%.d: ${SRC}/%.c |${BUILD}
	${CXX} -MG -MM ${CXXFLAGS} $< -MF $@.temp ${INCLUDES}
	@echo -n "${BUILD}/" > $@
	@cat $@.temp >> $@
	@rm $@.temp

${BUILD}/%.d: ${SRC}/%.S |${BUILD}
	${AS} -MG -MM ${ASMFLAGS} $< -MF $@.temp ${INCLUDES}
	@echo -n "${BUILD}/" > $@
	@cat $@.temp >> $@
	@rm $@.temp

${BUILD}/%.o: ${SRC}/%.cpp ${BUILD}/%.d |${BUILD}
	@echo "(CXX   ) $(notdir $<) -> $(notdir $@)"
	${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: ${SRC}/%.c ${BUILD}/%.d |${BUILD}
	@echo "(CC    ) $(notdir $<) -> $(notdir $@)"
	${CC} -c ${CFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: ${SRC}/%.S ${BUILD}/%.d |${BUILD}
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

${BIN}/%.elf: ${BUILD}/%.o |${BIN}
	@echo "(LD    ) $(notdir $<) -> $(notdir $@)"
	${CXX} ${LDFLAGS} $< -o $@ ${LDPATHS} ${LIBS}

${BIN}/%.hex: ${BIN}/%.elf |${BIN}
	@echo "(OBJCP ) $(notdir $<) -> $(notdir $@)"
	${OBJCP} ${OBJCPFLAGS} -O ihex $< $@

${LIB}/lib${LIBNAME}.a: ${OBJECTS} | ${LIB} 
	@echo "(AR    ) $@ <- $(notdir ${OBJECTS})"
	$(AR) ${ARFLAGS} $@ ${OBJECTS}

%.program: ${BIN}/%.hex
	@echo "(FLASH)  $(notdir $<) -> ${PORT} -> ${CHIP}"
	${FLASHER} ${FLASHOPTS} -P ${PORT} -p ${CHIP} -c ${PROGRAMMER} -U f:w:$<:i

${PMGENBIN}:
	@make -C $(dir ${PMGENBIN})

-include ${DEPS}
