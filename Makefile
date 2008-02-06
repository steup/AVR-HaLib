INCLUDE = ./include
BUILDDIR = ./build
CTRLS = atmega32 at90can128

CC = avr-g++
CFLAGS = -Wall -g -Os -I $(INCLUDE)

# CPP-Dateien
CPP_FILES = $(shell ls ./src/*.cpp)
# Dateinamen ohne Verzeichnis und Endung
SRCLIST = $(notdir $(CPP_FILES:.cpp=))

.PHONY: all
.PHONY: docs clean
.PHONY: $(CTRLS)

all: $(CTRLS)

docs:
	doxygen docs/Doxyfile

# grep zum rausfiltern von irrelevanten warnings
$(CTRLS): % : $(BUILDDIR) $(BUILDDIR)/%
	@echo ======= Compiling halib for $@ in $(BUILDDIR)/$@ =======
	@for obj in $(SRCLIST); do echo "------- Compiling $$obj for $@ -------"; $(CC) $(CFLAGS) -c ./src/$$obj.cpp -o $(BUILDDIR)/$@/$$obj.o -mmcu=$@; done
	@echo ======= Generating $(BUILDDIR)/libhalib-$@.a =======
	@ar rc $(BUILDDIR)/libhalib-$@.a $(BUILDDIR)/$@/*.o
	
$(BUILDDIR):
	@mkdir $@

$(BUILDDIR)/%:
	@mkdir $@
	
clean:
	@echo Cleaning
	@rm -rf $(BUILDDIR)/*
