#------- USER_CONFIG -------------

# Tools used
CC=avr-gcc
CXX=avr-g++
AS=avr-gcc
AR=avr-ar
LD=avr-ld
OBJCP=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size

# Chip dependent configuration

#Chip type
CHIP?=at90can128

# Clock frequency in Hz
CLOCK?=16000000

# Config for flashing
FLASHER?=avrdude
PROGRAMMER?=avr911
FLASHOPTS?=
PORT?=/dev/ttyUSB0

HALIB?=$(abspath ${RULESDIR}/..)

#----------END USER_CONFIG --------
