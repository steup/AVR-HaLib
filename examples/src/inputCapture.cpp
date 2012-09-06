//#define LOGGING_DISABLE

#define DYNAMIC 0

#include "platform.h"

#include <avr-halib/avr/InterruptManager/InterruptManager.h>
#include <avr-halib/avr/inputCaptureUnit.h>
#include <avr-halib/avr/clock.h>

using avr_halib::driver::InputCaptureUnit;

struct CaptureConf : public InputCaptureUnit::DefaultConfig
{
#if !DYNAMIC
    static const InterruptType intType = InterruptTypes::fixed;
#endif
    static const bool noiseCancel      = true;
};

typedef InputCaptureUnit::configure<CaptureConf>::type Capture;

Capture capture;

void onCapture()
{
    log::emit() << "got something at " << capture.value() << log::endl;
}

#if !DYNAMIC
typedef Capture::StaticCallbackType::bind< &onCapture > StaticCallback;

typedef Capture::setCallback< StaticCallback >::InterruptSlotList InterruptSlotList;
#else
typedef Capture::InterruptSlotList InterruptSlotList;
#endif

typedef Interrupt::InterruptManager< InterruptSlotList > IM;

int main() 
{
    IM::init();
    log::emit() << "Input Capture test" << log::endl;

#if DYNAMIC
    Capture::CallbackType cb;
    cb.bind<&onCapture>();

    capture.setCallback(cb);
#endif
    capture.enable();
    sei();

	while (1);
	return 0;	
}


