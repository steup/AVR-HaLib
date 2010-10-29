# Makefile for avr-halib
#
# targets: all, clean, docs, portmaps, portmapgen, library
# author:  Philipp Werner, Christoph Steup
# date:    20.05.2008
#
# This file is part of avr-halib. See COPYING for copyright details.

.DEFAULT_GOAL ?= all

include rules/loadconfig.mk

DOC=./docs
LIB=./build
BUILD=./build/${CHIP}
SRC=./src
PORTMAPDIR=./include/avr-halib/portmaps
INC=${PORTMAPDIR}
HALIB=.

LIBNAME=avr-halib-${CHIP}

.PHONY: all docs clean examples portmapgen portmaps experimental

all: externals portmaps ${LIB}/lib${LIBNAME}.a

include rules/general.mk
include rules/externals.mk

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
	@rm -rf ${GENDIRS} ${DOC}/html $(filter-out ${INC}/bobbyboard.h, $(wildcard *.dump ${INC}/*_portmap.h ${INC}/*.h))
	@make clean -C $(dir ${PMGENBIN})
	@make clean -C ./examples/applications
	@make clean -C ./examples/experimental

#Small workaround to be compatible with old names of generated portmaps
${PORTMAPS}: ${PORTMAPDIR}/%.h: ${PORTMAPDIR}/%_portmap.h
	@cp $< $@

portmaps: ${PORTMAPS}
