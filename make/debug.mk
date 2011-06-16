GDB_SERVER?=avarice
GDB_CLIENT?=avr-gdb

GDB_CLIENT_OPTS+= -x ${HALIB}/rules/gdb.script
GDB_SERVER_OPTS+=-e -p -j usb -2 -f
GDB_PORT?=9090

.PHONY: %.debug_server %.debug

%.debug_server: ${BIN}/%.elf
	${GDB_SERVER} ${GDB_SERVER_OPTS} $< :${GDB_PORT}

%.debug: ${BIN}/%.elf
	${GDB_CLIENT} ${GDB_CLIENT_OPTS} $<
