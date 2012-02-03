.PHONY: %.program

${BIN}/%.hex: ${BIN}/%.elf |${BIN}
	@echo "(IHEX ) $(notdir $<) -> $(notdir $@)"
	@${OBJCP} ${OBJCPFLAGS} -O ihex $< $@

%.program: ${BIN}/%.hex
	@echo "(FLASH)  $(notdir $<) -> ${PORT} -> ${CHIP}"
	@${FLASHER} ${FLASHOPTS} -P ${PORT} -p ${CHIP} -c ${PROGRAMMER} -U f:w:$<:i
