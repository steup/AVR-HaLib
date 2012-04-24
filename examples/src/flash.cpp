#include <platform.h>

#include <avr-halib/avr/flash.h>
#include <avr-halib/ext/button.h>

typedef avr_halib::drivers::Flash::configure<>::type Flash;
typedef avr_halib::drivers::Flash::ContentType ContentType;
typedef avr_halib::drivers::Flash::FuseContentType Fuses;
typedef avr_halib::drivers::Flash::LockContentType Locks;
typedef avr_halib::ext::Button<Button0> EraseButton;
typedef avr_halib::ext::Button<Button1> ProgramButton;

static const char* const content="Testing";

int main()
{
    EraseButton erase;
    ProgramButton program;
    Fuses fuseValue;
    Locks lockValue;
    Flash::readFuses(fuseValue);
    Flash::readLocks(lockValue);
    
    log::emit() << "Low  fuse: " << (uint16_t)fuseValue.lowFuses  << log::endl; 
    log::emit() << "high fuse: " << (uint16_t)fuseValue.highFuses << log::endl; 
    log::emit() << "ext  fuse: " << (uint16_t)fuseValue.extFuses  << log::endl; 
    log::emit() << "Locks    : " << (uint16_t)lockValue.locks     << log::endl; 
    log::emit() << "Waiting for button 0" << log::endl;
    while(!erase.isPressed());
    Flash::Writer writer;
    Flash::Reader reader;
    Flash::Address index=writer.getCurrentAddress();
    while(index.getPage()<4)
    {
        writer.setCurrentAddress(index);
        writer.eraseCurrentPage();
        index.nextPage();
    }
    log::emit() << "Waiting for button 1" << log::endl;
    for(uint16_t i=0;i<1024;i+=2)
    {
        ContentType buffer;
        reader.read(buffer);
        log::emit() << (char)(buffer) << (char)(buffer>>8);
    }
    log::emit() << log::endl;
    while(!program.isPressed());
    writer.setCurrentAddress(0);
    writer.eraseCurrentPage();
    for(uint16_t i=0;i<1024;i+=2)
        writer.write(((uint16_t)content[(i+1)%8]<<8)|content[i%8]);
    log::emit() << "Done" << log::endl;
    reader.setCurrentAddress(0);
    for(uint16_t i=0;i<1024;i+=2)
    {
        ContentType buffer;
        reader.read(buffer);
        char c1 = (char)(buffer&0x7f);
        char c2 = (char)((buffer>>8)&0x7f);
        if(c1)
            log::emit() << c1;
        else
            log::emit() << log::endl;
        if(c2)
            log::emit() << c2;
        else
            log::emit() << log::endl;
    }

    return 0;
}
