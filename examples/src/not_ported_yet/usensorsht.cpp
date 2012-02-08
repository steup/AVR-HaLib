/**
 *	\file	examples/application/usensorsht.cpp
 *	\brief	Example illustrating usage of sensor classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "sht_portmap.h"

#include "avr-halib/common/syncsensor.h"
#include "avr-halib/ext/sht.h"

using avr_halib::sensor::SyncSensor;

int main()
{
	typedef SHTfront SHT;
	
	SyncSensor< SHTTemperatur< SHT > > temp;
	SyncSensor< SHTHumidity< SHT > > hum;	


    log::emit() << "Reset! Messungen: 4 3 2 1" << log::endl;
	while(true)
	{
        log::emit() << (int16_t)temp.getValue() << "\t" << (int16_t)hum.getValue() << log::endl;	
		delay_ms(1000);
	}
    return 0;
}
