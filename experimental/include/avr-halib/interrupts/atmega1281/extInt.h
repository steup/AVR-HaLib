#pragma once

#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	template<uint8_t nr>
	struct ExternalInterruptIntMap
	{
		static const uint8_t baseNumber = 1;

		enum Interrupts
		{
			externalInterrupt= baseNumber + nr,
		};

		typedef ::Interrupt::Slot<externalInterrupt, ::Interrupt::Binding::DynamicPlainFunction> ExternalInterruptSlot;
		typedef typename boost::mpl::vector<ExternalInterruptSlot>::type Slots;
	};
}

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<0>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<0>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT0_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT0_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT0_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<1>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<1>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT1_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT1_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT1_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<2>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<2>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT2_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT2_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT2_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<3>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<3>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT3_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT3_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT3_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<4>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<4>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT4_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT4_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT4_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<5>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<5>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT5_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT5_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT5_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<6>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<6>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT6_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT6_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT6_vect, F, obj);
	}
};

template<>
struct InterruptRegistration<at90can128::ExternalInterruptIntMap<7>, false>
{
	private:
		typedef at90can128::ExternalInterruptIntMap<7>::Interrupts Int;

	public:
	template<typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		redirectISRM(INT7_vect, F, obj);
	}

	template<void (*F)(void)>
	static void registerCallback()
	{
		redirectISRF(INT7_vect, F);
	}

	template<typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		redirectISRM(INT7_vect, F, obj);
	}
};

}
}
