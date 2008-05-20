# Makefile for portmaps
#
# targets: all / portmaps, clean
# author:  Philipp Werner
# date:    20.05.2008

# Location of portmap generator
PMAPGENDIR = ../../../tools/portmapgen/
PMAPGEN	= ../../../tools/portmapgen/portmapgen

# Portmap-Files
PORTMAP_FILES = $(shell ls *.portmap)
# Files without directory and extentions
PORTMAP_LIST = $(notdir $(PORTMAP_FILES:.portmap=.h))


.PHONY: all portmaps clean


all: portmaps

portmaps: $(PORTMAP_LIST)

%.h : %.portmap $(PMAPGEN)
	@echo ========== Make portmap $@ ==========
	$(PMAPGEN) -o $@ $<

clean:
	rm $(PORTMAP_LIST)

# Make portmap generator
$(PMAPGEN):
	make -C $(PMAPGENDIR)
