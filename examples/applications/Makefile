# Tagets: X for all files X.cpp, clean, fuses, all

INCLUDE  = ../../include
LIB      = ../../build
BUILDDIR = ./build
PMAPGEN  = ../../tools/portmapgen/avr-halib-pmg

# Fill in you AVR MCU type...
CTRL	= at90can128
# CTRL	= atmega32

# Select or add your AVRDUDE_FLAGS
AVRDUDE_FLAGS =  -v -P /dev/ttyUSB0 -u -c avr911 -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P usb -u -c avrisp2 -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P /dev/parport0 -c stk200 -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P /dev/pcan0 -u -c pcan -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P /dev/pcan32 -u -c pcan -p $(CTRL)

CC      = avr-g++
CFLAGS  = -Wall -g  -Os -I $(INCLUDE) -mmcu=$(CTRL)
OC      = avr-objcopy
OD      = avr-objdump
SIZE	= avr-size
LDFLAGS = -j .text -j .data

CPP_FILES = $(shell ls *.cpp)
ELF_FILES = $(CPP_FILES:.cpp=.elf)

TARGETS = $(CPP_FILES:.cpp=)
STARGETS = fuses clean

AVRDUDE = avrdude


.PHONY: all $(STARGETS)

all:	$(ELF_FILES)

digitalio.elf : digitalio.cpp
	@echo "----- preprocessing digitalio.cpp (portmap generator) -----"
	$(PMAPGEN) -o digitalio.pp.cpp digitalio.cpp	
	@echo "----- compiling & linking" digitalio.pp.cpp "-----"
	$(CC) $(CFLAGS) -o $(BUILDDIR)/digitalio.elf digitalio.pp.cpp -L$(LIB) -lavr-halib-$(CTRL)
	

%.elf:%.cpp  $(BUILDDIR)
	@echo "----- compiling & linking" $*.cpp "-----"
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$*.elf $*.cpp -L$(LIB) -lavr-halib-$(CTRL)


%.hex:%.elf  $(BUILDDIR)
	@echo "----- generate" $*.hex "-----"
	$(OC) $(LDFLAGS) -O ihex $(BUILDDIR)/$*.elf $(BUILDDIR)/$*.hex

%.S:%.elf  $(BUILDDIR)
	@echo "----- generate" $*.hex "-----"
	$(OD) -dSCe $(BUILDDIR)/$*.elf >$(BUILDDIR)/$*.S
	$(SIZE) $(BUILDDIR)/$*.elf

$(TARGETS): %: %.hex
	
	@echo "----- programing the device -----"
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U f:w:$(BUILDDIR)/$@.hex:a

fuses:
	#fusebytes für at90can128
	avrdude -P /dev/parport0 -c stk200 -p at90can128 -U lfuse:w:0xef:m
	avrdude -P /dev/parport0 -c stk200 -p at90can128 -U hfuse:w:0xd9:m
	avrdude -P /dev/parport0 -c stk200 -p at90can128 -U efuse:w:0xff:m


clean:
	@echo "Cleaning"
	@rm -rf $(BUILDDIR)/*


$(BUILDDIR):
	@mkdir $(BUILDDIR)
