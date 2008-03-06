
#include <stdint.h>
#include <avr/io.h>


#define F_CPU 16000000


void delay_ms(uint16_t ms) __attribute__ ((naked));

void delay_ms(uint16_t ms)
{
// 	for (; ms; ms--)
// 	{
// 		// wait for one ms
// 		for (register uint16_t c = (uint16_t)(F_CPU / 5000); c; c--)
// 		{
// 			asm volatile ("nop" : : );
// 		}
// 	}
	asm volatile
	(
		"rjmp    .+14           	; jump to l3			\n"
/* l1: */	"ldi     r18, lo8(%1)   	; load loop count		\n"
		"ldi     r19, hi8(%1)    					\n"
/* l2: */	"nop								\n"
		"subi    r18, 0x01		; decrement (16 bit)		\n"
		"sbci    r19, 0x00						\n"
		"brne    .-8			; jump to l2 if not zero	\n"
		"sbiw    %0, 0x01		; decrement (16 bit)		\n"
/* l3: */	"sbiw    %0, 0x00						\n"
		"brne    .-18			; jump to l1 if not zero"
		:				// no output
		: "w" (ms), "i" (F_CPU / 5000)	// input: ms to wait and loop runs per ms
	);
}



int main()
{
	DDRA |= 1;
	while (1)
	{
		PORTA = ((PORTA & 1) ^ 1) | (PORTA & ~1);
		delay_ms(100);
	}
}
