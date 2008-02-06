INCLUDE  = ../../include
BUILDDIR = ./build
CTRL	= at90can128
# CTRL	= atmega32

#AVRDUDE_FLAGS =  -v -P /dev/parport0 -c stk200 -p $(CTRL)
AVRDUDE_FLAGS =  -v -P /dev/pcan0 -u -c pcan -p $(CTRL)
#AVRDUDE_FLAGS =  -v -P /dev/ttyUSB0 -u -c avr911 -p $(CTRL)

CC      = avr-g++
CFLAGS  = -Wall -g -Os -I $(INCLUDE) -mmcu=$(CTRL)
OC      = avr-objcopy
LDFLAGS = -j .text -j .data

TARGETS = led uart usensor umotor timer
STARGETS = fuses clean

AVRDUDE = avrdude


.PHONY: all fuses clean

all:
	@echo
	@echo Bitte eines der folgenden Ziele wählen 
	@echo $(TARGETS) $(STARGETS)

%.hex:	%.cpp  $(BUILDDIR)
	@echo "compiling & linking"
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$*.elf $*.cpp
	@echo "generate properly output format"
	$(OC) $(LDFLAGS) -O ihex $(BUILDDIR)/$*.elf $(BUILDDIR)/$*.hex
# link: -L../build -lhalib-$(CTRL)


$(TARGETS): %: %.hex
	
	@echo "Programing the device."
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
