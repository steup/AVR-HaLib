INCLUDE  = ../include
BUILDDIR = ./build
#CTRL	= at90can128
CTRL	= atmega32
STK	= stk200

CC      = avr-g++
CFLAGS  = -Wall -g -Os -I $(INCLUDE) -mmcu=$(CTRL)
OC      = avr-objcopy
LDFLAGS = -j .text -j .data

TARGETS = led uart
STARGETS = fuses clean

AVRDUDE = avrdude

#ifeq ($(OSTYPE),linux)
	AVRDUDE_FLAGS =  -v -P /dev/parport0
#else
#	AVRDUDE_FLAGS =  -P lpt1
#endif
AVRDUDE_FLAGS += -c $(STK)
AVRDUDE_FLAGS += -p $(CTRL)

.PHONY: all fuses clean

all:
	@echo
	@echo Bitte eines der folgenden Ziele wählen 
	@echo $(TARGETS) $(STARGETS)

%.hex:	%.cpp  $(BUILDDIR)
	@echo "compiling & linking"
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$*.elf $*.cpp -L../build -lhalib-$(CTRL)
	
	@echo "generate properly output format"
	$(OC) $(LDFLAGS) -O ihex $(BUILDDIR)/$*.elf $(BUILDDIR)/$*.hex


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
	@mkdir $(BUILDDIR)#TARGET  = $1
