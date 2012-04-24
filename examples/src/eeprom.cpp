#include "platform.h"

#include <avr-halib/regmaps/local.h>
#include <avr-halib/regmaps/base/eeprom.h>
#include <avr-halib/regmaps/base/remoteRegMap.h>
#include <boost/mpl/list.hpp>

using avr_halib::regmaps::base::RemoteRegMap;
using avr_halib::regmaps::base::EEPROM;
using avr_halib::regmaps::base::RWModes;
using avr_halib::regmaps::base::RWModeType;
using boost::mpl::list;

struct EEPROMContent
{
    struct RunCount
    {
        static const uint16_t address = 0x0;
        static const RWModeType mode  = RWModes::both;

        uint64_t runs;
    };

    typedef boost::mpl::list< RunCount >::type RegisterList;
};

typedef EEPROM::configure<>::type Interface;

typedef RemoteRegMap::configure< EEPROMContent, Interface >::type RunCounter;

int main()
{
	UseRegMap(counter, RunCounter);

	log::emit() << "EEPROM read test" << log::endl;
	if(SyncRegister(counter, RunCounter::Registers::RunCount))
		log::emit() << "Run count: " << counter.runs << log::endl;
	else
		log::emit() << "eeprom busy" << log::endl;

	log::emit() << "EEPROM write test" << log::endl;
	counter.runs++;
	if(SyncRegister(counter, RunCounter::Registers::RunCount))
		log::emit() << "write successfull" << log::endl;
	else
		log::emit() << "eeprom busy" << log::endl;
	
	return 0;
}
