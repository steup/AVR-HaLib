#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <stdint.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{
	namespace helpers
	{
		template<uint8_t address, uint8_t offset>
		struct HWMutexGenerator : public base::LocalRegMap
		{
			private:
				uint8_t _pad[address];

			public:
				uint8_t       : offset;
				uint8_t value : 1;
		} __attribute__((packed));
	}
	
	typedef helpers::HWMutexGenerator<0x3E,  0> HardwareMutex0;
	typedef helpers::HWMutexGenerator<0x3E,  1> HardwareMutex1;
	typedef helpers::HWMutexGenerator<0x3E,  2> HardwareMutex2;
	typedef helpers::HWMutexGenerator<0x3E,  3> HardwareMutex3;
	typedef helpers::HWMutexGenerator<0x3E,  4> HardwareMutex4;
	typedef helpers::HWMutexGenerator<0x3E,  5> HardwareMutex5;
	typedef helpers::HWMutexGenerator<0x3E,  6> HardwareMutex6;
	typedef helpers::HWMutexGenerator<0x3E,  7> HardwareMutex7;
	typedef helpers::HWMutexGenerator<0x4A,  8> HardwareMutex8;
	typedef helpers::HWMutexGenerator<0x4A,  9> HardwareMutex9;
	typedef helpers::HWMutexGenerator<0x4A, 10> HardwareMutex10;
	typedef helpers::HWMutexGenerator<0x4A, 11> HardwareMutex11;
	typedef helpers::HWMutexGenerator<0x4A, 12> HardwareMutex12;
	typedef helpers::HWMutexGenerator<0x4A, 13> HardwareMutex13;
	typedef helpers::HWMutexGenerator<0x4A, 14> HardwareMutex14;
	typedef helpers::HWMutexGenerator<0x4A, 15> HardwareMutex15;
	typedef helpers::HWMutexGenerator<0x4B, 16> HardwareMutex16;
	typedef helpers::HWMutexGenerator<0x4B, 17> HardwareMutex17;
	typedef helpers::HWMutexGenerator<0x4B, 18> HardwareMutex18;
	typedef helpers::HWMutexGenerator<0x4B, 19> HardwareMutex19;
	typedef helpers::HWMutexGenerator<0x4B, 20> HardwareMutex20;
	typedef helpers::HWMutexGenerator<0x4B, 21> HardwareMutex21;
	typedef helpers::HWMutexGenerator<0x4B, 22> HardwareMutex22;
	typedef helpers::HWMutexGenerator<0x4B, 23> HardwareMutex23;
}
}
}
}
