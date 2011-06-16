# Makefile for avr-halib
#
# targets: all, clean, docs, portmaps, portmapgen, library
# author:  Philipp Werner, Christoph Steup
# date:    20.05.2008
#
# This file is part of avr-halib. See COPYING for copyright details.

# load default/user config
include config.mk

DOC=./docs
LIB=./lib
BUILD=./build/${CHIP}
SRC=./src
PORTMAPDIR=./include/avr-halib/portmaps
INC=${PORTMAPDIR}
HALIB=.

PORTMAPS=$(addsuffix .h, $(basename $(wildcard ${PORTMAPDIR}/*.portmap)))

LIBNAME=avr-halib-${CHIP}

.PHONY: all docs clean examples portmapgen portmaps experimental

all: externals portmaps ${LIB}/lib${LIBNAME}.a ./build/lib${LIBNAME}.a

./build/lib${LIBNAME}.a: ${LIB}/lib${LIBNAME}.a
	ln -s ../$< $@

include ./config.mk
include make/config.mk
include make/rules.mk
include make/externals.mk

GARBAGE+=./build ${LIB}

${PMGENBIN}:
	@make -C $(dir ${PMGENBIN})

examples: externals ${PORTMAPS} ${LIB}/lib${LIBNAME}.a
	@echo ========== Making example programs ==========
	@make all -C ./examples/applications

experimental: externals ${PORTMAPS} ${LIB}/lib${LIBNAME}.a
	@echo ========== Making experimental example programs ==========
	@make all -C ./examples/experimental

docs: 
	@echo ========== Making HTML documentation ==========
	@doxygen ${DOC}/Doxyfile

clean: externals_clean
	@echo ========== Cleaning ==========
	@echo "(CLEAN )"
	@rm -rf ${GARBAGE} ${DOC}/html $(filter-out ${INC}/bobbyboard.h, $(wildcard *.dump ${INC}/*_portmap.h ${INC}/*.h))
	@make clean -C $(dir ${PMGENBIN})
	@make clean -C ./examples/applications
	@make clean -C ./examples/experimental

#Small workaround to be compatible with old names of generated portmaps
${PORTMAPS}: ${PORTMAPDIR}/%.h: ${PORTMAPDIR}/%_portmap.h
	@cp $< $@

portmaps: ${PORTMAPS}
