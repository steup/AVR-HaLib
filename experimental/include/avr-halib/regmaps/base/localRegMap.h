#pragma once

namespace avr_halib
{
namespace regmaps
{
namespace base
{
	/**\brief Register Map base class
	 * 
	 * This class contains functionality that is needed by all local register maps
	 **/
	struct LocalRegMap
	{
		/**\brief Sync the local register map against the memory
		 * \return always true
		 **/
		bool sync()
		{
			asm volatile("":::"memory");
			return true;
		}

		/**\brief Sync one register against the memory
		 * \tparam T the register, that should be synchronized
		 * \param unused just a placeholder to pass the type T
		 * \return always true
		 **/
		template<typename T>
		bool sync(T* unused)

		{
			asm volatile("":::"memory");
			return true;
		}
	};
}
}
}
