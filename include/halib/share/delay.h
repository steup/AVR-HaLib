
#define F_CPU 16000000

void delay_ms(uint16_t ms)
{
	// 16000000 1/s
	for (; ms; ms--)
	{
		// Je eine ms
		for (uint16_t c = (uint16_t)(F_CPU / 1000U); c; c--)
		{
			asm volatile ("nop" : : );
		}
	}
}
