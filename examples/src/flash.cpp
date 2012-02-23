#include "config.h"

#include <avr-halib/avr/flash.h>

typedef avr_halib::drivers::Flash::configure<>::type Flash;

int main()
{
    Flash::FlashReader reader;
    Flash::FlashWriter writer;
    log::emit() << "FlashWrite" << log::endl;
    writer.setCurrentAddress(10000);
    for(unsigned int i=0;i<1000;i++)
      writer.write(0x00);

    reader.setCurrentAddress(10000);
    for(unsigned int i=0;i<256;i++)
      log::emit() << log::hex << reader.read();
    return 0;
}
