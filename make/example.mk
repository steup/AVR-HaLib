include ${HALIB}/make/config.mk
include ${HALIB}/make/rules.mk

EXAMPLES=$(notdir $(basename ${OBJECTS}))

PORTMAPS=$(foreach pm,$(wildcard ${INC}/*.portmap),$(basename ${pm})_portmap.h)

GARBAGE+=${PORTMAPS}

.PHONY: help all generate_portmaps %.program ${EXAMPLES}

help:
	@echo "current examples: "
	@echo all ${EXAMPLES}

all: $(addprefix ${BIN}/, $(addsuffix .elf, ${EXAMPLES}))

generate_portmaps: ${PORTMAPS}

${BIN}/%.elf: ${BUILD}/%.o | ${BIN} generate_portmaps
	@echo "(LD    ) $(notdir $<) -> $(notdir $@)"
	@${CXX} ${LDFLAGS} $< -o $@ ${LDPATHS} ${LIBS}

${EXAMPLES}: %: ${BIN}/%.elf

clean:
	@echo "(CLEAN)"
	@rm -rf ${GARBAGE}
