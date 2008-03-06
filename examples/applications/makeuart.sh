avr-g++ -Wall -g  -I ../../include -mmcu=at90can128 -O3 -o ./build/uart.elf uart.cpp ../../src/avr/interrupt.S
avr-objcopy -j .text -j .data -O ihex ./build/uart.elf ./build/uart.hex
avrdude -v -P /dev/ttyUSB0 -u -c avr911 -p at90can128 -U f:w:./build/uart.hex:a
