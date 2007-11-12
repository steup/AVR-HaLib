#include ../misc.h

void main() {
	LedBlock leds(PORTA, DDRA, 0x0f ,0);
	leds.setLedPattern(0xf);
}

