DBG_SERVER      ?= avarice
DBG_CLIENT      ?= avr-gdb

DBG_CLIENT_OPTS += -x ${HALIB}/rules/gdb.script
DBG_SERVER_OPTS += -e -p -j usb -2 -f
DBG_PORT        ?= 9090
DBG_KEEPER      ?= ${MISC}/dgb_running

.PHONY: %.debug_server %.debug

${DBG_KEEPER}:
	@echo "Starting GDB server on Port: ${DBG_PORT}"
	touch $@ && ${DBG_SERVER} ${DBG_SERVER_OPTS} $< :${DBG_PORT} || rm $@ &

%.debug: ${BIN}/%.elf DBG_KEEPER
	@echo "Starting GDB connected to Port: ${DBG_PORT}"
	@${DBG_CLIENT} ${DBG_CLIENT_OPTS} $<
