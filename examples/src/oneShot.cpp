#include <platform.h>

#include <avr-halib/ext/led.h>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/insert_range.hpp>
#include <avr-halib/avr/clock.h>
#include <avr-halib/avr/oneShotTimer.h>
#include <avr-halib/common/frequency.h>

using boost::mpl::int_;
using avr_halib::config::Frequency;

struct ClockConfig
{
    typedef avr_halib::regmaps::local::Timer3 RegMap;
    typedef platform::CPUClock InputFrequency;
    typedef Frequency<1, 2> TargetFrequency;
    typedef uint16_t TickValueType;
};

typedef avr_halib::drivers::Clock<ClockConfig> Clock;

typedef avr_halib::drivers::OneShotTimer::configure<>::type OneShot;

typedef boost::mpl::insert_range< OneShot::InterruptSlotList, boost::mpl::begin<OneShot::InterruptSlotList>::type, Clock::InterruptSlotList >::type InterruptList;

typedef Interrupt::InterruptManager< InterruptList > IM;

typedef avr_halib::ext::Led< platform::Led0 > LED0;
typedef avr_halib::ext::Led< platform::Led1 > LED1;
typedef avr_halib::ext::Led< platform::Led2 > LED2;

LED0 led0;
LED1 led1;
LED2 led2;
OneShot oneShot;
Clock clock;

void one()
{
    led0.toggle();
    Clock::Time now;
    clock.getTime(now);
    log::emit() << "1," << now.microTicks << log::endl;
}

void two()
{
    led1.toggle();
    Clock::Time now;
    clock.getTime(now);
    log::emit() << "2," << now.microTicks << log::endl;
}

void three()
{
    led2.toggle();
    Clock::Time now;
    clock.getTime(now);
    log::emit() << "3," << now.microTicks << log::endl;
}

void doIt()
{
    oneShot.setup<OneShot::Units::matchA>(500);
    oneShot.setup<OneShot::Units::matchB>(1000);
    oneShot.setup<OneShot::Units::matchC>(1500);
    Clock::Time now;
    clock.getTime(now);
    log::emit() << "c," << now.ticks << "," << now.microTicks << log::endl;
}

int main()
{
	IM::init();

    OneShot::CallbackType cbA;
    OneShot::CallbackType cbB;
    OneShot::CallbackType cbC;
    Delegate<void> cbClock;

    cbClock.bind<doIt>();
    cbA.bind<one>();
    cbB.bind<two>();
    cbC.bind<three>();

    oneShot.setCallback<OneShot::Units::matchA>(cbA);
    oneShot.setCallback<OneShot::Units::matchB>(cbB);
    oneShot.setCallback<OneShot::Units::matchC>(cbC);
    clock.setCallback(cbClock);

	sei();

    while(true)
        Morpheus::sleep(Morpheus::SleepModes::idle);
	return 0;
}

