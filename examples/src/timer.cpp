#include <platform.h>

#include <avr-halib/drivers/avr/timer.h>
#include <avr-halib/drivers/ext/led.h>
#include <avr-halib/interrupts/interrupt.h>

using namespace avr_halib::regmaps::local;
using namespace avr_halib::config;
using namespace avr_halib::drivers;
using ext::Led;

typedef Timer2 MyTimer;

struct TimerConfig : public TimerDefaultConfig<MyTimer>
{
    enum Parameters
    {
        overflowInt = true,
        ocmAInt     = true,
        async       = true,
    };

    static const MyTimer::Prescalers ps = MyTimer::ps1024;
};

typedef avr::Timer<TimerConfig> Timer;

typedef Led< platform::Led0 > Led0;
typedef Led< platform::Led1 > Led1;

Led0 led0;
Led1 led1;

using namespace avr_halib::interrupts::interrupt_manager;
using boost::mpl::vector;

typedef  Slot<Timer::InterruptMap::matchA, Binding::FixedObjectFunction>::Bind<Led0, &Led0::toggle, led0> MatchASlot;
typedef  Slot<Timer::InterruptMap::overflow, Binding::FixedObjectFunction>::Bind<Led1, &Led1::toggle, led1> OverflowSlot;

typedef vector<MatchASlot, OverflowSlot>::type IntList;

typedef InterruptManager< IntList > IM;

BIND_INTERRUPTS(IM);

int main()
{
    /*Timer::InterruptMap::OverflowSlot::bind< Led0, &Led0::toggle >( &led0 );
    Timer::InterruptMap::MatchASlot::bind  < Led1, &Led1::toggle >( &led1 );*/

    Timer timer;

    timer.setOutputCompareValue<Timer::Units::matchA>(128);

    sei();
    timer.start();

    while(true)
        Morpheus::sleep<Morpheus::SleepModes::powerSave>();

    return 0;
}
