PORTMAP_DIR  := ${BUILD}/portmaps
PORTMAP_SRCS := $(foreach src, ${PM_SRC_DIRS}, $(wildcard ${src}/*.portmap))
PORTMAPS     := $(foreach pm, ${PORTMAP_SRCS}, ${PORTMAP_DIR}/$(notdir $(basename ${pm}))_portmap.h)
GENDIRS      += ${PORTMAP_DIR}
TO_CLEAN     += ${PORTMAP_DIR}

INCLUDES     += ${PORTMAP_DIR}

vpath %.portmap ${PM_SRC_DIRS}

.PHONY: portmaps

portmaps: ${PORTMAPS}

${PORTMAP_DIR}/%_portmap.h: %.portmap | ${PORTMAP_DIR} ${PMGEN}
	@echo "(PMGEN ) $(notdir $<) -> $(notdir $@)"
	@${PMGEN} $< > $@

