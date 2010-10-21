RULESDIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

include ${RULESDIR}/default.mk

include ${HALIB}/config.mk

$(SHELL echo help)

include $(wildcard config.mk)

%.mk:|%.mk.src
	cp $@.src $@

config.mk:|${RULESDIR}/config.mk
	cp ${RULESDIR}/config.mk $@

%.mk:
	make -C $(dir $@) $(notdir $@)


