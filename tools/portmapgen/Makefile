TARGET = portmapgen
BUILD = build

all:  $(TARGET)


$(TARGET) : portmapgen.cpp portmapgen.h uc.cpp uc.h parse.cpp parse.h
	g++ -o portmapgen portmapgen.cpp uc.cpp parse.cpp
