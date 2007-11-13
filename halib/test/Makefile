INCLUDE	= ..
BUILDDIR	= ./build
CTRL	= atmega32
STK	= stk200

CC      = avr-g++
CFLAGS  = -Wall -g -Os -I $(INCLUDE) -mmcu=$(CTRL)
OC      = avr-objcopy
LDFLAGS = -j .text -j .data

AVRDUDE = avrdude

AVRDUDE_FLAGS =	-P /dev/parport0
AVRDUDE_FLAGS += -c $(STK)
AVRDUDE_FLAGS += -p $(CTRL)
AVRDUDE_FLAGS += -U
.PHONY: all
.PHONY: clean

clean:
	@echo "Cleaning"
	@rm -rf $(BUILDDIR)/*

halib:
	$(CC) $(CFLAGS) -c *.cpp
	ar rc libhalib.a *.o
	
	


led:	$(BUILDDIR) halib
	
	@echo "compiling & linking"
	@$(CC) $(CFLAGS) -o $(BUILDDIR)/$@.elf $@.cpp -L.. -lhalib
	
	@echo "generate properly output format"
	@$(OC) $(LDFLAGS) -O ihex $(BUILDDIR)/$@.elf $(BUILDDIR)/$@.hex
	@echo "Programing the device."
	$(AVRDUDE) $(AVRDUDE_FLAGS) f:w:$(BUILDDIR)/$@.hex:a
	
	
$(BUILDDIR) :
	mkdir $(BUILDDIR)