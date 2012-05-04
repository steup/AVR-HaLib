include ${HALIB_DIR}/make/pmgen.mk
include ${HALIB_DIR}/make/portmaps.mk
include ${HALIB_DIR}/make/externals.mk
include ${HALIB_DIR}/make/lib.mk

CFLAGS    := ${CFLAGS}   $(filter-out ${CFLAGS},   ${AVR_CFLAGS})
CXXFLAGS  := ${CXXFLAGS} $(filter-out ${CXXFLAGS}, ${AVR_CXXFLAGS})
ASMFLAGS  := ${ASMFLAGS} $(filter-out ${ASMFLAGS}, ${AVR_ASMFLAGS})
LDFLAGS   := ${LDFLAGS}  $(filter-out ${LDFLAGS},  ${AVR_LDFLAGS})

ALL_BUILD := ${BUILD}
BUILD     := ${BUILD}/${TARGET}
GENDIRS   += ${BUILD}
SOURCES   := $(wildcard ${SRC}/*.cpp ${SRC}/*.cc ${SRC}/*.c ${SRC}/*.S)
LDPATHS   := $(addprefix -L,${LDPATHS})
LIBS      := $(addprefix -l,${LIBS})
INCLUDES  := $(addprefix -I,${INCLUDES})
OBJECTS   := $(foreach OBJ, ${SOURCES}, ${BUILD}/$(notdir $(basename ${OBJ})).o)
DEPS      := $(wildcard ${BUILD}/*.d)
TO_CLEAN  += ${BUILD} ${BIN}
TO_DCLEAN += ${ALL_BUILD}

TO_CLEAN  := $(sort ${TO_CLEAN})
TO_DCLEAN := $(sort ${TO_DCLEAN})
GENDIRS   := $(sort ${GENDIRS})

.PHONY: clean distclean
.PRECIOUS: ${BUILD}/%.d ${BUILD}/%.o

vpath %.c   ${SRC}
vpath %.cc  ${SRC}
vpath %.cpp ${SRC}
vpath %.S   ${SRC}


${GENDIRS}: %:
	@mkdir -p $@

${BUILD}/%.d: %.cpp | ${BUILD}
	@${CXX} -MT ${BUILD}/$*.o -MG -MM ${CXXFLAGS} $< ${INCLUDES} -MF $@ 

${BUILD}/%.d: %.cc  | ${BUILD}
	@${CXX} -MT ${BUILD}/$*.o -MG -MM ${CXXFLAGS} $< ${INCLUDES} -MF $@ 

${BUILD}/%.d: %.c   | ${BUILD}
	@${CC}  -MT ${BUILD}/$*.o -MG -MM ${CFLAGS} $< ${INCLUDES} -MF $@ 

${BUILD}/%.d: %.S   | ${BUILD}
	@${AS}  -MT ${BUILD}/$*.o -MG -MM ${ASMFLAGS} $<  ${INCLUDES} -MF $@

${BUILD}/%.o: %.cpp ${BUILD}/%.d | ${ADDITIONAL_DEPS} ${BUILD}
	@echo "(CXX   ) $(notdir $<) -> $(notdir $@)"
	@${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: %.cc  ${BUILD}/%.d | ${ADDITIONAL_DEPS} ${BUILD}
	@echo "(CXX   ) $(notdir $<) -> $(notdir $@)"
	@${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: %.c   ${BUILD}/%.d | ${ADDITIONAL_DEPS} ${BUILD}
	@echo "(CC    ) $(notdir $<) -> $(notdir $@)"
	@${CC} -c ${CFLAGS} $< -o $@ ${INCLUDES}

${BUILD}/%.o: %.S   ${BUILD}/%.d | ${ADDITIONAL_DEPS} ${BUILD}
	@echo "(AS    ) $(notdir $<) -> $(notdir $@)"
	@${AS} -c ${ASMFLAGS} $< -o $@ ${INCLUDES}

-include ${DEPS}
