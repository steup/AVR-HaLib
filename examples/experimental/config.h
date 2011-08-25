#pragma once

#include "logConf.h"

#include <avr-halib/regmaps/local.h>
#include <avr-halib/regmaps/remote/srf10.h>
#include <avr-halib/portmaps/robbyboard.h>

#include <avr-halib/avr/InterruptManager/InterruptManager.h>
#include <avr-halib/ext/sensorPowerControl.h>

#include <boost/mpl/vector.hpp>

static const bool useOldRobbyBoard=true;
static const uint16_t lowFreqCrystal = 32768;

using namespace avr_halib;

typedef InterruptManager<boost::mpl::vector<>::type, false> NoInt;

typedef drivers::robby::SensorPowerControl<SensorPowerSupply, useOldRobbyBoard> SensorPower;

typedef regmaps::local::ADCRegMap< CPUClock > ADCRegMap;

typedef regmaps::remote::SRF10RegMap< TWI< CPUClock > > SRF10RegMap;

struct ClockConfig
{
	typedef uint16_t TickValueType;
	typedef Frequency<1> TargetFrequency;
//	typedef CPUClock TimerFrequency;
	typedef Frequency< lowFreqCrystal > TimerFrequency;
	typedef regmaps::local::Timer2 Timer;
};

typedef power::Morpheus<MorpheusSyncList> Morpheus;
