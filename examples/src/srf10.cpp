#include "config.h"

#include <avr-halib/ext/srf10.h>

typedef typename avr_halib::drivers::SRF10::configure<>::type DistanceSensor;

DistanceSensor sensor;

int main()
{
	uint8_t pos, errPos;

	log::emit() << log::clear;
	log::emit() << "Version: " << (uint16_t)sensor.getVersion() << log::endl;
	log::emit() << "US: "      << log::getPos(pos)              << log::endl;
	log::emit() << "Error: "   << log::getPos(errPos);

	if( !sensor.setGain(0) )
		log::emit() << log::setPos( errPos ) << "Gain";
	if( !sensor.setRange(10) )
		log::emit() << log::setPos( errPos ) << "Range";

	while(true)
	{
		if( !sensor.startMeasurement( DistanceSensor::Units::cm ) )
			log::emit() << log::setPos( errPos ) << "Measure";

		while( sensor.isBusy() );

		log::emit() << log::setPos( pos ) << "     ";
		log::emit() << log::setPos( pos ) << sensor.fetchResult();
	}
	return 0;
}
