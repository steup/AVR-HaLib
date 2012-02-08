#pragma once

#include "register.h"

#include <stdint.h>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>

#include <avr-halib/common/mpl.h>

namespace avr_halib
{
namespace regmaps
{
namespace base
{
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
    struct RemoteRegMap
    {
        private:
            /**\brief Empty class as start value for the merge operation of the register list**/
            struct Empty
            {
                /** \brief End of List Constructor
                 *
                 * Takes the communication interface instance, but does nothing with it.
                 * This Constructor marks the end of the recursion through the register list.
                 *
                 * /tparam Interface the communication interface to use
                 * /param iface Reference to the instance of the communication interface
                 **/
                template<typename Interface>
                Empty(Interface &iface){}
            };

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
                    {
                        /** \brief Pass communication interface instance
                         * This constructor passes the communication interface instance to the current register in the list and to the rest of the list
                         *
                         * /tparam Interface the communication interface to use
                         * /param iface Reference to the instance of the communication interface
                         **/
                        template<typename Interface>
                        type(Interface &iface) : curr(iface), Register<next, next::mode>(iface)
                        {
                        
                        }
                    }; 
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
                        /**\brief template metafunction to merge a register list into one class
             * \tparam RegList the register list
             **/
            template<typename RegList>
            struct merge
            {
                /**\brief the result of the operation**/
                typedef typename mpl::fold<RegList, Empty, concatOp>::type type;
            };

        public:

        template<typename RegisterDescription, typename Interface>
        struct configure
        {
            private:    

                typedef typename RegisterDescription::RegisterList RegisterList;
                typedef typename merge< RegisterList >::type       MergedRegisters;

                
            public:

                struct type : private Interface, public MergedRegisters
                {
                    public:
                  
                        /**\brief Configuration dependant parameters**/
                        enum ConfigurationParameters
                        {
                            numReg = mpl::size< RegisterList >::value,	/**<the number of registers in this regmap**/
                            size   = getSize< RegisterList >::type::value /**<the sum of all registers` size**/
                        };

                        typedef RegisterDescription Registers;

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
                                template<typename  CurReg>
                                void operator()(CurReg &r)
                                {
                                    Register<CurReg, CurReg::mode>* reg=reinterpret_cast<Register<CurReg, CurReg::mode>*>(&rm);
                                    if(result && !reg->sync(rm))
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
                        /** \brief Construct the regmap by fetching remote register values
                         *
                         * This constructor fetches the remote register values by passing an
                         * instance of the communication interface to the merged remote
                         * register list.
                         **/
                        type() : MergedRegisters(*this)
                        {
                            
                        }

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
                            typedef typename select<RegisterList, reg>::type currReg;
                            return currReg::sync(*this);
                        }

                        /**\brief Synchronize all register
                         * \return the result of this sync operation, true if everything went ok, false otherwise
                         **/
                        bool sync()
                        {
                            Synchronisator s(*this);
                            mpl::for_each<RegisterList>(s);
                            return s.getResult();
                        }

                    template<typename, RWModeType> friend class Register;
                };
        };
    };

}
}
}
