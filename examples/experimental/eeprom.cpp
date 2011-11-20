#include "logConf.h"

#include <avr-halib/regmaps/local.h>
#include <avr-halib/regmaps/base/eeprom.h>
#include <avr-halib/regmaps/base/remoteRegMap.h>
#include <boost/mpl/list.hpp>

using avr_halib::regmaps::base::RemoteRegMap;
using avr_halib::regmaps::base::EepromAccess;
using avr_halib::regmaps::local::Eeprom;
using boost::mpl::list;

struct RunCount
{
	enum RegisterParameters
	{
		address=0,
		mode=avr_halib::regmaps::base::both
	};
	uint64_t runCount;
};

typedef RemoteRegMap< EepromAccess< Eeprom >, 
		     list< RunCount >::type 
		    > EepromContent;

int main()
{
	UseRegMap(eeprom, EepromContent);

	log::emit() << "EEPROM read test" << log::endl;
	if(SyncRegister(eeprom, RunCount))
		log::emit() << "Run count: " << eeprom.runCount << log::endl;
	else
		log::emit() << "eeprom busy" << log::endl;

	log::emit() << "EEPROM write test" << log::endl;
	eeprom.runCount++;
	if(SyncRegister(eeprom, RunCount))
		log::emit() << "write successfull" << log::endl;
	else
		log::emit() << "eeprom busy" << log::endl;
	
	return 0;
}
