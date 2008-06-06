# Makefile for portmap generation tool
#
# targets: all, clean
# author:  Philipp Werner
# date:    20.05.2008

TARGET = avr-halib-pmg
BUILD = build

all:  $(TARGET)


$(TARGET) : portmapgen.cpp portmapgen.h uc.cpp uc.h parse.cpp parse.h generate.cpp generate.h
	@echo ========== Make portmap generator ==========
	g++ -o2 -o $(TARGET) portmapgen.cpp uc.cpp parse.cpp generate.cpp
	
clean :
	rm $(TARGET)
	
