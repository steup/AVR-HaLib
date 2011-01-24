EXAMPLES=$(notdir $(basename ${OBJECTS}))

GARBAGE+=$(wildcard ${INC}/*_portmap.h)

.PHONY: help all %.program ${EXAMPLES}

default:
	@echo "current examples: "
	@echo all ${EXAMPLES}

all: $(addprefix ${BIN}/, $(addsuffix .elf, ${EXAMPLES}))

${EXAMPLES}: %: ${BIN}/%.elf

clean:
	@echo "(CLEAN)"
	@rm -rf ${GARBAGE}
