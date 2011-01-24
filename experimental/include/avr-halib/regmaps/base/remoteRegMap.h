#pragma once

#include <stdint.h>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>

#include <avr-halib/share/mplExt.h>

namespace avr_halib
{
namespace regmaps
{
namespace base
{

namespace mpl=avr_halib::mplExt;

/**\brief Possible register access modes**/
enum RWMode
{
	read=1,		/**< Register is readable**/
	write=2,	/**< Register is writable**/
	both=3		/**< Register is readable and writable**/
};

namespace helpers
{

/**\brief Content length value check helper
 * \tparam value true is ok, false generate error
 **/
template<bool value>
struct InvalidContentLength;

/**\brief Template specalization, if value check is successfull**/
template<>
struct InvalidContentLength<true>{};

/**\brief Select an appropriate alias type for a register
 * \tparam size the size of the register
 *
 * This metafunction selects an appropriate alias type from the uint*_t family
 * to represent the content of a register
 **/

template<unsigned int size>
struct contentAliasTypeSelector
{
	/**\brief Check for 64bit register**/
	typedef typename mpl::if_c< (size<=8), uint64_t, InvalidContentLength<false> >::type check64;
	/**\brief Check for 32bit register**/
	typedef typename mpl::if_c< (size<=4), uint32_t, check64 					 >::type check32;
	/**\brief Check for 16bit register**/
	typedef typename mpl::if_c< (size<=2), uint16_t, check32 					 >::type check16;
	/**\brief Check for 8bit register**/
	typedef typename mpl::if_c< (size==1), uint8_t , check16 					 >::type type;
};

/**\brief One register of a Distributed RegMap
 * \tparam content a content description of this register
 * \tparam mode the R/W mode of this register, \see RWMode
 *
 * This class provides an appropriate implementation of the register
 * synchronization dependent on the R/W type of the register
 **/
template<typename content, uint8_t mode>
struct Register;

/**\brief One register of a Distributed RegMap
 * \tparam content a content description of this register
 * \tparam mode the R/W mode of this register, \see RWMode
 *
 * This class provides an implementation synchronization for read-only registers.
 **/
template<typename content>
struct Register<content, read> : public content
{
	/**\brief Forward declaration of the register content**/
	typedef content Content;

	/**\brief Define the alias type pf this register**/
	typedef typename contentAliasTypeSelector<sizeof(Content)>::type Alias;

	/**\brief Compile-time constant parameter values**/
	enum ConstantParameters
	{
		size=sizeof(content)	/**<Size of the register in bytes**/
	};

	/**\brief Sync the read-only register
	 * \tparam Interface the bus interface to use
	 * \param iface an instance of the bus interface
	 *
	 * This function synchronizes the content of the local copy of the register
	 * with ist remote counterpart, by communicating with the remote device via
	 * the provided bus interface.
	 **/
	template<typename Interface>
	bool sync(Interface &iface)
	{
		return iface.read(this->address, reinterpret_cast<Alias&>(*this));
	}
};

/**\brief Template specalization for write-only registers
 * \tparam content a content description of this register
 *
 * This class provides an implementation of synchronization for write-only
 * registers
 **/
template<typename content>
struct Register<content, write> : public content
{
	/**\brief Forward declaration of the register content**/
	typedef content Content;

	/**\brief Define the alias type pf this register**/
	typedef typename contentAliasTypeSelector<sizeof(Content)>::type Alias;

	/**\brief Compile-time constant parameter values**/
	enum ConstantParameters
	{
		size=sizeof(content)	/**<Size of the register in bytes**/
	};

	/**\brief Sync the write-only register
	 * \tparam Interface the bus interface to use
	 * \param iface an instance of the bus interface
	 *
	 * This function synchronizes the content of the local copy of the register
	 * with ist remote counterpart, by communicating with the remote device via
	 * the provided bus interface.
	 **/
	template<typename Interface>
	bool sync(Interface &iface)
	{
		return iface.write(this->address, reinterpret_cast<const Alias&>(*this));
	}
};

/**\brief Template specalization for read-write registers
 * \tparam content a content description of this register
 *
 * This class provides an implementation of synchronization for read-write
 * registers
 **/
template<typename content>
struct Register<content, both> : public content
{
	/**\brief Forward declaration of the register content**/
	typedef content Content;

	/**\brief Define the alias type pf this register**/
	typedef typename contentAliasTypeSelector<sizeof(Content)>::type Alias;

	/**\brief Compile-time constant parameter values**/
	enum ConstantParameters
	{
		size=sizeof(content)	/**<Size of the register in bytes**/
	};

	/**\brief A copy of the registers content, to sasve changed bits**/
	Alias oldValue;

	/**\brief Default constructor
	 *
	 * Initialize the content of the copy with current content of the register
	 **/
	Register()
	{
		oldValue=*reinterpret_cast<Alias*>(static_cast<content*>(this));
	}

	/**\brief Sync the read-write register
	 * \tparam Interface the bus interface to use
	 * \param iface an instance of the bus interface
	 *
	 * This function synchronizes the content of the local copy of the register
	 * with its remote counterpart, by communicating with the remote device via
	 * the provided bus interface. It changes only the bits of the register,
	 * that changed in between the last synchronization.  The current value of
	 * all unchanged bits will be updated too.
	 **/
	template<typename Interface>
	bool sync(Interface &iface)
	{
		Alias diff = reinterpret_cast<Alias>(*this) ^ oldValue;

		if(!iface.read(this->address, oldValue))
			return false;

		oldValue |= diff & reinterpret_cast<Alias>(*this);
		oldValue &= ~( diff & ~reinterpret_cast<Alias>(*this) );

		if(!iface.write(this->address, oldValue))
			return false;

		reinterpret_cast<Alias>(*this)=oldValue;

		return true;
	}
};

/**\brief Empty class as start value for the merge operation of the register list**/
struct Empty{};

/**\brief the used concatenation operation for register list merging
 *
 * It merges one register list into one class, that contains all the contents
 * of the registers of the list.  
 **/
struct concatOp
{
	/**\brief the worker metafunction
	 * \tparam curr the currently merged register list
	 * \tparam next the next element to merge
	 **/
	template<typename curr, typename next>
	struct apply
	{
		/**\brief result of the local merging**/
		struct type : public curr, public Register<next, next::mode>
		{}; 
	};
};

/**\brief the user operation to calculate the size of the register list**/
struct getSizeOp
{
	/**\brief the worker metafunction
	 * \tparam curr the current size
	 * \tparam next the next register in the list
	 **/
	template<typename curr, typename next>
	struct apply
	{
		/**\brief the result of the local size calculation**/
		typedef typename mpl::int_<curr::value+sizeof(next)>::type type;
	};
};

/**\brief template metafunction to merge a register list into one class
 * \tparam RegList the register list
 **/
template<typename RegList>
struct merge
{
	/**\brief the result of the operation**/
	typedef typename mpl::fold<RegList, Empty, concatOp>::type type;
};

/**\brief template metafunction to calculate the size of all registers in a list**/
template<typename RegList>
struct getSize
{
	/**\brief the resulting size as an compile time int**/
	typedef typename mpl::fold<RegList, mpl::int_<0>, getSizeOp>::type type;
};

/**\brief template metafunction to select a register out of a register list
 * \tparam RegList the register list
 * \tparam regDesc the content description of the register
 *
 * This metafunction selects the appropriate register out of the register list,
 * that matches the provided register content
 **/
template<typename RegList, typename regDesc>
struct select
{
	/**\brief The result of the search in the list**/
	typedef typename mpl::find<RegList, regDesc>::type result;
	/**\brief Dereferencing the result iterator**/
	typedef typename mpl::deref<result>::type currDesc;
	/**\brief Convert the resulting register description into a register**/
	typedef Register<currDesc, currDesc::mode> type;
};

}

/**\brief One register accessed via a bus
 * \tparam content the content of this register
 * \tparam mode the access mode of this register \see RWMode
 *
 * This struct represent a register, whichs content is changed locally, but
 * upon synchronisation is distributed over a bus to a target hardware
 **/
template<typename content, uint8_t mode>
struct Register : public helpers::Register<content, mode>{};

/**\brief A Distributed RegMap abstracting hardware access over a bus
 * \tparam IF the bus interface to use
 * \tparam RegList the list of register contents of the target
 *
 * This class represents a register mapping from a remote hardware, connected
 * by a bus.  It enables the user to locally change the content of the register
 * of the target and transmit aggregated changes upon synchronisation.  The
 * whole implementation is designed to be consistent to the original local
 * RegMao concept, so that driver written for the local RegMap concept will
 * also work remotely, but a performence penalty is likely, if the SyncRegister
 * macro is not used.
 **/
template<typename IF, typename RegList>
struct RemoteRegMap : public helpers::merge<RegList>::type
{
	public:

		/**\brief the configuration of this drm**/
		struct config
		{
			/**\brief declare the used bus interface**/
			typedef IF Interface;
			/**\brief declare which registers are used**/
			typedef RegList Registers;
		};

		
		/**\brief Configuration dependant parameters**/
		enum ConfigurationParameters
		{
			numReg=mpl::size<RegList>::value,	/**<the number of registers in this regmap**/
			size=helpers::getSize<RegList>::type::value /**<the sum of all registers` size**/
		};

	private:
		/**\brief Shortcut to the used bus interface**/
		typedef IF Interface;
		/**\brief Shrotcut to the final merges register list;**/
		typedef typename helpers::merge<RegList>::type Registers;

	private:
		/**\brief Internal worker struct, that synchronizes all registers**/
		struct Synchronisator
		{
			private:
				/**\brief An instance of this Distributed RegMap, to access the bus**/
				RemoteRegMap& rm;
				/**\brief the result of the synchronisation operation**/
				bool result;

			public:
				/**\brief Construct the worker.
				 * \param rm An instance of this Distributed RegMap
				 *
				 * Initialize the result to be true and set the reference to the DRM
				 **/
				Synchronisator(RemoteRegMap& rm) : rm(rm), result(true){}

				/**\brief the "callback" for the for_each function
				 * \tparam Register, the register that should be synchronized
				 * \param r an instance of Register
				 *
				 * This function executes the synchronisation, and updates the
				 * result value dependent on the result of this synchronisation
				 * operation.
				 **/
				template<typename Register>
				void operator()(Register &r)
				{
					if(!rm.Register::sync(Interface::getInstance()))
						result=false;
				}
				
				/**\brief Get the result of the last synchronisation of all registers
				 * \return true if everything went ok, false otherwise
				 **/
				bool getResult()
				{
					return result;
				}
		};
		
	public:
		/**\brief Synchronize one register
		 * \tparam reg the register, that should be synchronized
		 * \param unused dummy parameter for nicer syntax
		 * \return the result of the sync operation, true if everything went ok, false otherwise
		 *
		 * This function is used by the SyncRegister makro. User should use
		 * this makro instead of calling this function direct.
		 **/
		template<typename reg>
		bool sync(reg *unused)
		{
			typedef typename helpers::select<RegList, reg>::type currReg;
			return currReg::sync(Interface::getInstance());
		}

		/**\brief Synchronize all register
		 * \return the result of this sync operation, true if everything went ok, false otherwise
		 **/
		bool sync()
		{
			Synchronisator s(*this);
			mpl::for_each<Registers>(s);
			return s.getResult();
		}
};

}
}
}
