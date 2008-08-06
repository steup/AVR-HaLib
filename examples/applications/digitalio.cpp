
#include "avr-halib/avr/digitalin.h"
#include "avr-halib/avr/digitalout.h"


#ifdef __AVR_AT90CAN128__
portmap Input for at90can128
{
	pins ipins : a 4-7;
	vport inPort { ipins };
	property usePullup = 0xf;
	property invertLevel = 0x5;	// 0000 -> 0101	
};

portmap Output for at90can128
{
	pins opins : a 0-3;
	vport outPort { opins };
	property initValue = 0x0;
	property invertLevel = 0x0;
};
#else
#	error "Portmap missing"
#endif

int main()
{
	DigitalIn<Input> in;
	DigitalOut<Output> out;

	while (1)
		out.set(in.get());
	
	return 0;
}



