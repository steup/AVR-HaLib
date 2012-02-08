#pragma once

#include <avr-halib/share/interruptLock.h>

namespace avr_halib
{
namespace locking
{
	template<typename HWMutexRegMap>
	struct HardwareMutex
	{
		static bool aquire()
		{
			UseRegMap(rm, HWMutexRegMap);
			SyncRegMap(rm);

			GlobalIntLock lock;

			if(rm.value)
				return false;

			rm.value=true;
			SyncRegMap(rm);

			return true;
		}

		static void release()
		{
			UseRegMap(rm, HWMutexRegMap);
			rm.value=false;
			SyncRegMap(rm);
		}
	};
}
}
