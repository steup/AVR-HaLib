FLASHER      ?= avrdude
FLASHER      := @${FLASHER}
PORT         ?= usb
PROGRAMMER   ?= avrisp2

FLASHOPTS    := -P ${PORT} -p ${TARGET} -c ${PROGRAMMER} -U f:w:
OBJCPYFLAGS  := -j .data -j .text -O ihex

.PHONY: %.program

${BIN}/%.hex: ${BIN}/%.elf |${BIN}
	@echo "(IHEX ) $(notdir $<) -> $(notdir $@)"
	@${OBJCPY} ${OBJCPYFLAGS} $< $@

%.program: ${BIN}/%.hex
	@echo "(FLASH)  $(notdir $<) via ${PORT} -> ${TARGET}"
	@${FLASHER} ${FLASHOPTS}$<
