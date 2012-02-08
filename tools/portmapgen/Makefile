# Makefile for portmap generation tool
#
# targets: all, clean
# author:  Philipp Werner
# date:    20.05.2008
#
# This file is part of avr-halib. See COPYING for copyright details.

TARGET = ./bin/pmgen

all:  $(TARGET)

vpath %.cpp ./src
vpath %.h   ./include

$(TARGET) : portmapgen.cpp portmapgen.h uc.cpp uc.h parse.cpp parse.h generate.cpp generate.h
	@mkdir -p bin
	@echo Building portmap generator
	@echo "(CXX   ) $@ <- $^"
	@g++ -O2 -o $(TARGET) ./src/portmapgen.cpp ./src/uc.cpp ./src/parse.cpp ./src/generate.cpp -I ./include

clean :
	rm -rf ./bin

