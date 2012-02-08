#pragma once

namespace avr_halib
{
namespace regmaps
{
namespace base
{
    struct RWModes
    {
        /**\brief Possible register access modes**/
        enum RWModeType
        {
            read  = 1, /**< Register is readable**/
            write = 2, /**< Register is writable**/
            both  = 3  /**< Register is readable and writable**/
        };
    };
    typedef RWModes::RWModeType RWModeType;

    /**\brief One register of a Distributed RegMap
     * \tparam content a content description of this register
     * \tparam mode the R/W mode of this register, \see RWMode
     *
     * This class provides an appropriate implementation of the register
     * synchronization dependent on the R/W type of the register
     **/
    template<typename content, RWModeType mode>
    struct Register;

    /**\brief One register of a Distributed RegMap
     * \tparam content a content description of this register
     * \tparam mode the R/W mode of this register, \see RWMode
     *
     * This class provides an implementation synchronization for read-only registers.
     **/
    template<typename content>
    struct Register<content, RWModes::read> : public content
    {
        /**\brief Forward declaration of the register content**/
        typedef content Content;

        /**\brief Compile-time constant parameter values**/
        enum ConstantParameters
        {
            size=sizeof(content)	/**<Size of the register in bytes**/
        };

        /**\brief Construct and initialize register by fetching remote value
         *
         * Initialize the content with current value of the remote register.
         *
         * /tparam Interface the communication interface to use
         * /param iface Reference to the instance of the communication interface
         **/
        template<typename Interface>
        Register(Interface& iface)
        {
            /*Alias temp;
            iface.read(this->address, temp);
            *reinterpret_cast<Alias*>(this)=temp;*/
        }

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
            return iface.read(this->address, reinterpret_cast<uint8_t*>(static_cast<content*>(this)), size);
        }
    };

    /**\brief Template specalization for write-only registers
     * \tparam content a content description of this register
     *
     * This class provides an implementation of synchronization for write-only
     * registers
     **/
    template<typename content>
    struct Register<content, RWModes::write> : public content
    {
        /**\brief Forward declaration of the register content**/
        typedef content Content;

        /**\brief Compile-time constant parameter values**/
        enum ConstantParameters
        {
            size=sizeof(content)	/**<Size of the register in bytes**/
        };

        /** Empty Constructor
         *
         * nothing to do here
         *
         * /tparam Interface unused
         * /param iface unused
         **/
        template<typename Interface>
        Register(Interface& iface){}

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
            return iface.write(this->address, reinterpret_cast<uint8_t*>(static_cast<content*>(this)), size);
        }
    };

    /**\brief Template specalization for read-write registers
     * \tparam content a content description of this register
     *
     * This class provides an implementation of synchronization for read-write
     * registers
     **/
    template<typename content>
    struct Register<content, RWModes::both> : public content
    {
        /**\brief Forward declaration of the register content**/
        typedef content Content;

        /**\brief Compile-time constant parameter values**/
        enum ConstantParameters
        {
            size=sizeof(content)	/**<Size of the register in bytes**/
        };

        /**\brief variable representing initialization status**/
        bool init : 1;

        /**\brief Construct and initialize register by fetching remote value
         *
         * Initialize the content and the copy with current value of the remote
         * register.
         *
         * /tparam Interface the communication interface to use
         * /param iface Reference to the instance of the communication interface
         **/
        template<typename Interface>
        Register(Interface& iface) : init(false){}

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
            if(init)
            {
                if(!iface.write(this->address, 
                           reinterpret_cast<uint8_t*>(static_cast<content*>(this)), 
                           size))
                    return false;
            }

            if(!iface.read(this->address, 
                       reinterpret_cast<uint8_t*>(static_cast<content*>(this)), 
                       size))
                return false;
            
                init=true;

            return true;
        }
    };
}
}
}
