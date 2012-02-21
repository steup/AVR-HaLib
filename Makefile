# Makefile for avr-halib
#
# targets: all, clean, docs, portmaps, portmapgen, library
# author:  Philipp Werner, Christoph Steup
# date:    20.05.2008
#
# This file is part of avr-halib. See COPYING for copyright details.

# load default/user config

HALIB_DIR := .

DOC       := ${HALIB_DIR}/doc


.PHONY: all doc clean distclean examples

all: examples

examples:
	@echo ========== Making example programs ==========
	@${MAKE} all -C ./examples

doc: 
	@echo ========== Making HTML documentation ==========
	@doxygen ${DOC}/Doxyfile

clean:
	@${MAKE} clean -C ./examples

distclean:
	@${MAKE} distclean -C ./examples
	@${MAKE} clean -C ./tools/bootloader
	@echo "Wiping documentation"
	@rm -rf ${DOC}/html
