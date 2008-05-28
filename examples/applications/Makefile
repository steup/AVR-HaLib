INCLUDE  = ../../include
LIB      = ../../build
BUILDDIR = ./build

CTRL	= at90can128
# CTRL	= atmega32

HALIB = ../..

#AVRDUDE_FLAGS =  -v -P /dev/parport0 -c stk200 -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P /dev/pcan0 -u -c pcan -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P /dev/pcan32 -u -c pcan -p $(CTRL)
AVRDUDE_FLAGS =  -v -P /dev/ttyUSB0 -u -c avr911 -p $(CTRL)

CC      = avr-g++
CFLAGS  = -Wall -g  -O3 -I $(INCLUDE) -mmcu=$(CTRL)
#-Os
OC      = avr-objcopy
LDFLAGS = -j .text -j .data

CPP_FILES = $(shell ls *.cpp)
ELF_FILES = $(CPP_FILES:.cpp=.elf)

TARGETS = $(CPP_FILES:.cpp=)
STARGETS = fuses clean

AVRDUDE = avrdude


.PHONY: all fuses clean

all:	$(ELF_FILES)



%.elf:%.cpp  $(BUILDDIR)
	@echo "----- compiling & linking" $*.cpp "-----"
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$*.elf $*.cpp -L$(LIB) -lhalib-$(CTRL)


%.hex:%.elf  $(BUILDDIR)
	@echo "----- generate" $*.hex "-----"
	$(OC) $(LDFLAGS) -O ihex $(BUILDDIR)/$*.elf $(BUILDDIR)/$*.hex

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
