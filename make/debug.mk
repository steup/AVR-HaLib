DBG_SERVER      ?= avarice
DBG_CLIENT      ?= avr-gdb

DBG_CLIENT_OPTS := -x
DBG_SERVER_OPTS := -e -p -j usb -2 -D -f 
DBG_PORT        ?= 9090
DBG_CONFIG_FILE    := ${BIN}/dbg.config
DBG_CONFIG_CONTENT := "target remote :${DBG_PORT}"

ifneq (${PROGRAMMER},jtag2)
	WARNING=@echo "Without a jtag 2 or similar no debugging is possible!" && false
endif

.PHONY: %.debug_server %.debug check_jtag

${DBG_CONFIG_FILE}: 
	@echo ${DBG_CONFIG_CONTENT} > $@

check_jtag:
	${WARNING}

%.debug: ${BIN}/%.elf | ${DBG_CONFIG_FILE} check_jtag
	@echo "(DBG  ) <- $(basename $(notdir $<))"
	@${DBG_SERVER} ${DBG_SERVER_OPTS} $< :${DBG_PORT}
	@${DBG_CLIENT} ${DBG_CLIENT_OPTS} ${DBG_CONFIG_FILE} $<
