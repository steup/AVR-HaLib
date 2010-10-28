RULESDIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

include ${RULESDIR}/default.mk

include ${HALIB}/config.mk

include $(wildcard config.mk)

config.mk ${HALIB}/config.mk:|${RULESDIR}/config.mk
	cp ${RULESDIR}/config.mk $@
