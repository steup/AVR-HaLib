#include "avr-halib/avr/uart.h"
#include "avr-halib/common/cdeviceframe.h"
#include "avr-halib/common/delay.h"

using avr_halib::drivers::Uart;
using avr_halib::regmaps::local::Uart1;

struct Config : public Uart::DefaultConfig
{
    typedef Uart1 RegMap;
    static const Uart::BaudRateType baudRate = 19200;
};

struct CFrameConfig : public CFrameModifierBase
{
	enum {esc = 'e', sofr = 'a', eofr = 'b', escmod = 0x7f};
};

typedef avr_halib::drivers::Uart::configure<Config>::type Uart;

typedef CFrame< CFrameConfig > Framing;
typedef CDeviceFrameNoInt< Uart, uint8_t, 255, Framing> FrameDevice;
typedef FrameDevice::mob_t mob_t;

FrameDevice fdev;
const char* aMsg = "01234567Test";

int main()
{
	mob_t message;
	for(message.size = 0; message.size < 12; message.size++)
		message.payload[message.size] = aMsg[message.size];

  	while(true)
    {
        fdev.send( message );
		delay_ms(50);
    }
}
