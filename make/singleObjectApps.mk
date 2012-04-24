ALL_BIN   := ${BIN}
BIN       := ${BIN}/${TARGET}
GENDIRS   += ${BIN}
TO_CLEAN  += ${BIN}
TO_DCLEAN += ${ALL_BIN}

.PHONY: default %.size %.dump

default: all

include ${HALIB_DIR}/make/rules.mk
include ${HALIB_DIR}/make/debug.mk
include ${HALIB_DIR}/make/flash.mk

APPS=$(notdir $(basename ${OBJECTS}))

.PHONY: help all ${APPS}

all: $(addprefix ${BIN}/, $(addsuffix .elf, ${APPS}))

${BIN}/%.elf: ${BUILD}/%.o ${PORTMAPS} ${LIB_NAME} | ${BIN} 
	@echo "(LD    ) $(notdir $<) -> $(notdir $@)"
	@${CXX} ${LDFLAGS} $< -o $@ ${LDPATHS} ${LIBS}

${APPS}: %: ${BIN}/%.elf

%.size:	${BIN}/%.elf
	@${SIZE} $<

%.dump: ${BIN}/%.elf
	@echo "(OBJDMP) $(notdir $<) -> $@"
	@${OBJDMP} -Cxd $< > $@
