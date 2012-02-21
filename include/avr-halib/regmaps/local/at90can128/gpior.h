#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace at90can128
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
	
	typedef helpers::HWMutexGenerator<0x3E,  0> SystemMutex0;
	typedef helpers::HWMutexGenerator<0x3E,  1> SystemMutex1;
	typedef helpers::HWMutexGenerator<0x3E,  2> SystemMutex2;
	typedef helpers::HWMutexGenerator<0x3E,  3> SystemMutex3;
	typedef helpers::HWMutexGenerator<0x3E,  4> SystemMutex4;
	typedef helpers::HWMutexGenerator<0x3E,  5> SystemMutex5;
	typedef helpers::HWMutexGenerator<0x3E,  6> SystemMutex6;
	typedef helpers::HWMutexGenerator<0x3E,  7> SystemMutex7;
	typedef helpers::HWMutexGenerator<0x4A,  8> HardwareMutex0;
	typedef helpers::HWMutexGenerator<0x4A,  9> HardwareMutex1;
	typedef helpers::HWMutexGenerator<0x4A, 10> HardwareMutex2;
	typedef helpers::HWMutexGenerator<0x4A, 11> HardwareMutex3;
	typedef helpers::HWMutexGenerator<0x4A, 12> HardwareMutex4;
	typedef helpers::HWMutexGenerator<0x4A, 13> HardwareMutex5;
	typedef helpers::HWMutexGenerator<0x4A, 14> HardwareMutex6;
	typedef helpers::HWMutexGenerator<0x4A, 15> HardwareMutex7;
	typedef helpers::HWMutexGenerator<0x4B, 16> HardwareMutex8;
	typedef helpers::HWMutexGenerator<0x4B, 17> HardwareMutex9;
	typedef helpers::HWMutexGenerator<0x4B, 18> HardwareMutex10;
	typedef helpers::HWMutexGenerator<0x4B, 19> HardwareMutex11;
	typedef helpers::HWMutexGenerator<0x4B, 20> HardwareMutex12;
	typedef helpers::HWMutexGenerator<0x4B, 21> HardwareMutex13;
	typedef helpers::HWMutexGenerator<0x4B, 22> HardwareMutex14;
	typedef helpers::HWMutexGenerator<0x4B, 23> HardwareMutex15;
}
}
}
}
