TARGET = portmapgen
BUILD = build

all:  $(TARGET)


$(TARGET) : portmapgen.cpp portmapgen.h uc.cpp uc.h parse.cpp parse.h generate.cpp generate.h
	g++ -o2 -o $(TARGET) portmapgen.cpp uc.cpp parse.cpp generate.cpp
	
clean :
	rm $(TARGET)
	
