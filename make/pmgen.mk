TOOL_BIN  := ${HALIB_DIR}/bin
PMGEN     := ${HALIB_DIR}/bin/pmgen
TO_DCLEAN += ${TOOL_BIN}
GENDIRS   += ${TOOL_BIN}

.PHONY: pmgen_distclean

${PMGEN}: | ${TOOL_BIN}
	${MAKE} -C ${HALIB_DIR}/tools/portmapgen/
	cp ${HALIB_DIR}/tools/portmapgen/bin/pmgen $@

pmgen_clean:
	${MAKE} -C tools/portmapgen/ clean
