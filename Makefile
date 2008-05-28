# Makefile for avr-halib
#
# targets: all, clean, docs, portmaps, portmapgen, $(CTRLS)
# author:  Philipp Werner
# date:    20.05.2008


INCLUDE = ./include
BUILDDIR = ./build
CTRLS = atmega32 at90can128

CC = avr-g++
CFLAGS = -Wall -g -Os -I $(INCLUDE)

## CPP-Files
#CPP_FILES = $(shell ls ./src/*.cpp)
## Files without directory and extention
#SRCLIST = $(notdir $(CPP_FILES:.cpp=))


.PHONY: all docs clean portmaps portmapgen examples $(CTRLS)


all: $(CTRLS) examples docs

docs:
	@echo ========== Making HTML documentation ==========
	doxygen docs/Doxyfile

portmaps:
	make -C ./include/avr-halib/portmaps

portmapgen:
	make -C ./tools/portmapgen

examples:
	@echo ========== Making example programs ==========
	make -C ./examples/applications

$(CTRLS): % : $(BUILDDIR) $(BUILDDIR)/% portmapgen portmaps
	@echo ========== Compiling avr-halib for $@ in $(BUILDDIR)/$@ ==========
	$(CC) $(CFLAGS) -c ./src/share/common.cpp -o $(BUILDDIR)/$@/common.o -mmcu=$@
	$(CC) $(CFLAGS) -c ./src/avr/interrupt.S -o $(BUILDDIR)/$@/interrupt.o -mmcu=$@
	@echo ========== Generating $(BUILDDIR)/libavr-halib-$@.a ==========
	ar rc $(BUILDDIR)/libavr-halib-$@.a $(BUILDDIR)/$@/*.o

# grep zum rausfiltern von irrelevanten warnings
#$(CTRLS): % : $(BUILDDIR) $(BUILDDIR)/%
#	@echo ======= Compiling avr-halib for $@ in $(BUILDDIR)/$@ =======
#	@for obj in $(SRCLIST); do echo "------- Compiling $$obj for $@ -------"; $(CC) $(CFLAGS) -c ./src/$$obj.cpp -o $(BUILDDIR)/$@/$$obj.o -mmcu=$@; done
#	@echo ======= Generating $(BUILDDIR)/libavr-halib-$@.a =======
#	@ar rc $(BUILDDIR)/libavr-halib-$@.a $(BUILDDIR)/$@/*.o
	
$(BUILDDIR):
	@mkdir $@

$(BUILDDIR)/%:
	@mkdir $@
	
clean:
	@echo ========== Cleaning ==========
	rm -rf $(BUILDDIR)/*
	make clean -C ./include/avr-halib/portmaps
	make clean -C ./tools/portmapgen
	make clean -C ./examples/applications
	
