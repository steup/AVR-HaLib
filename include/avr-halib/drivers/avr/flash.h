#pragma once

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr-halib/locking/interruptLock.h>
#include <avr-halib/regmaps/local.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** \brief Flash Driver
     *
     * TODO \todo
     */
    struct Flash
    {
        /** \brief TODO \todo */
        typedef uint16_t OffsetType;
        /** \brief TODO \todo */
        typedef uint16_t PageType;
        /** \brief TODO \todo */
        typedef uint16_t ContentType;
        /** \brief TODO \todo */
        typedef uint32_t LinearAddressType;

        /** \brief TODO \todo */
        union FuseContentType
        {
            /** \brief TODO \todo */
            uint32_t value;
            struct{
                uint8_t lowFuses;
                uint8_t highFuses;
                uint8_t extFuses;
            };
        };

        /** \brief TODO \todo */
        union LockContentType
        {
            /** \brief TODO \todo */
            uint8_t value;
            /** \brief TODO \todo */
            struct
            {
                uint8_t locks;
            };
        };
        /** \brief TODO \todo */
        static const uint32_t farAddressStart = 0x10000;

        /** \brief Flash default configuration */
        struct DefaultConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::Flash RegMap;

            /** \brief TODO \todo */
            static const OffsetType        pageSize   = SPM_PAGESIZE;
            /** \brief TODO \todo */
            static const LinearAddressType flashStart = 0;
            /** \brief TODO \todo */
            static const LinearAddressType appEnd     = 0x1d000;
            /** \brief TODO \todo */
            static const LinearAddressType flashEnd   = 0x1ffff;

            /** \brief TODO \todo */
            static const bool autoIncrement = true;
            /** \brief TODO \todo */
            static const bool autoWritePage = true;
            /** \brief TODO \todo */
            static const bool autoErase     = false;
            /** \brief TODO \todo */
            static const bool preserveUnset = false;
        };

        /** \brief Flash configuration
         *
         * \tparam cfg Configuration
         */
        template<typename Config = DefaultConfig>
        struct configure
        {
            /** \brief Flash base type */
            struct type
            {
                private:
                    /** \brief TODO \todo */
                    typedef typename Config::RegMap RegMap;

                public:
                    /** \brief TODO \todo */
                    typedef Config config;

                    /** \brief TODO \todo */
                    static const PageType appPageEnd   = Config::appEnd   / Config::pageSize;
                    /** \brief TODO \todo */
                    static const PageType flashPageEnd = Config::flashEnd / Config::pageSize;

                    /** \brief TODO \todo */
                    static void read(LinearAddressType address, uint8_t &value)
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts  %1, %2\n\t"
                                      "elpm %0,  Z\n\t"
                                      : "=r" (value)
                                      : "i" ((uint16_t)&rm.rampz),
                                        "r"  ((uint8_t)(address>>16)&0xff),
                                        "z"  ((uint16_t)address&0xffff)
                                     );
                    }

                    /** \brief TODO \todo */
                    static void read(LinearAddressType address, ContentType &value)
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts  %1 , %2 \n\t"
                                      "elpm %A0,  Z+\n\t"
                                      "elpm %B0,  Z \n\t"
                                      : "=r" (value)
                                      : "i"  ((uint16_t)&rm.rampz),
                                        "r"  ((uint8_t)(address>>16)&0xff),
                                        "z"  ((uint16_t)address&0xffff)
                                     );
                    }

                    /** \brief TODO \todo */
                    static void writeBuffer(OffsetType offset, ContentType data)
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "movw  r0, %2\n\t"
                                      "sts   %0, %1\n\t"
                                      "spm         \n\t"
                                      "clr   r1    \n\t"
                                      :
                                      : "i" ((uint16_t)&rm.spmcsr),
                                        "r" ((uint8_t)(RegMap::Commands::bufferWrite)),
                                        "r" (data),
                                        "z" (offset)
                                      : "r0"
                                    );
                    }

                    /** \brief TODO \todo
                     *
                     * TODO \todo
                     */
                    static void waitUntilDone()
                    {
                        UseRegMap(rm, RegMap);
                        while(rm.spmen)SyncRegMap(rm);
                        rm.rwwsb=true;
                        SyncRegMap(rm);
                    }

                    /** \brief TODO \todo
                     *
                     * TODO \todo
                     */
                    static void enableRWW()
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts %0, %1\n\t"
                                      "spm       \n\t"
                                      :
                                      : "i" ((uint16_t)&rm.spmcsr),
                                        "r" ((uint8_t)(RegMap::Commands::enableRWW))
                                    );
                    }

                    /** \brief TODO \todo
                     *
                     * \param page TODO \todo
                     */
                    static void erasePage(PageType page)
                    {
                        uint32_t pageExt = (uint32_t)page * Config::pageSize;
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts %1, %3\n\t"
                                      "sts %0, %2\n\t"
                                      "spm       \n\t"
                                      :
                                      : "i" ((uint16_t)&rm.spmcsr),
                                        "i" ((uint16_t)&rm.rampz),
                                        "r" ((uint8_t)(RegMap::Commands::pageErase)),
                                        "r" ((uint8_t)(pageExt>>16)&0xff),
                                        "z" ((uint16_t)(pageExt))
                                );
                    }

                    /** \brief TODO \todo
                     *
                     * \param page TODO \todo
                     */
                    static void writePage(PageType page)
                    {
                        uint32_t pageExt = (uint32_t)page * Config::pageSize;
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts %1, %3\n\t"
                                      "sts %0, %2\n\t"
                                      "spm       \n\t"
                                      :
                                      : "i" ((uint16_t)&rm.spmcsr),
                                        "i" ((uint16_t)&rm.rampz),
                                        "r" ((uint8_t)(RegMap::Commands::pageWrite)),
                                        "r" ((uint8_t)(pageExt>>16)&0xff),
                                        "z" ((uint16_t)(pageExt))
                                );
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    static void readFuses(FuseContentType& value)
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts %1, %2\n\t"
                                      "lpm %0,  Z\n\t"
                                      : "=r" (value.lowFuses)
                                      : "i"  ((uint16_t)&rm.spmcsr),
                                        "r"  ((uint8_t)RegMap::Commands::lockFuseAccess),
                                        "z"  ((uint16_t)RegMap::SpecialAddresses::lowFuse)
                                     );
                        asm volatile( "sts %1, %2\n\t"
                                      "lpm %0,  Z\n\t"
                                      : "=r" (value.highFuses)
                                      : "i"  ((uint16_t)&rm.spmcsr),
                                        "r"  ((uint8_t)RegMap::Commands::lockFuseAccess),
                                        "z"  ((uint16_t)RegMap::SpecialAddresses::highFuse)
                                     );
                        asm volatile( "sts %1, %2\n\t"
                                      "lpm %0,  Z\n\t"
                                      : "=r" (value.extFuses)
                                      : "i"  ((uint16_t)&rm.spmcsr),
                                        "r"  ((uint8_t)RegMap::Commands::lockFuseAccess),
                                        "z"  ((uint16_t)RegMap::SpecialAddresses::extFuse)
                                     );
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    static void readLocks(LockContentType& value)
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "sts %1, %2\n\t"
                                      "lpm %0,  Z\n\t"
                                      : "=r" (value.locks)
                                      : "i"  ((uint16_t)&rm.spmcsr),
                                        "r"  ((uint8_t)RegMap::Commands::lockFuseAccess),
                                        "z"  ((uint16_t)RegMap::SpecialAddresses::lock)
                                     );
                    }

                    /** \brief TODO \todo
                     *
                     * \param value TODO \todo
                     */
                    static void writeLock(LockContentType value)
                    {
                        UseRegMap(rm, RegMap);
                        asm volatile( "mov %0, r0\n\t"
                                      "sts %1, %2\n\t"
                                      "spm       \n\t"
                                      :
                                      : "r"  (value),
                                        "i"  ((uint16_t)&rm.spmcsr),
                                        "r"  ((uint8_t)RegMap::Commands::lockFuseAccess)
                                      : "r0"
                                );
                    }

                    /** \brief TODO \todo
                     *
                     * TODO \todo
                     */
                    struct Address
                    {
                        private:
                            /** \brief TODO \todo */
                            PageType page;
                            /** \brief TODO \todo */
                            OffsetType offset;

                        public:

                            Address() : page(0), offset(0){}

                            /** \brief TODO \todo
                             *
                             * \param address TODO \todo
                             */
                            Address(const LinearAddressType address)
                            {
                                page   = address / Config::pageSize;
                                offset = address % Config::pageSize;
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            bool isValid() const
                            {
                                return ( page       <= flashPageEnd     &&
                                         offset     <  Config::pageSize &&
                                         offset % 2 == 0 );
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            PageType getPage() const
                            {
                                return page;
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            OffsetType getOffset() const
                            {
                                return offset;
                            }

                            /** \brief TODO \todo
                             *
                             * \param offset TODO \todo
                             */
                            bool setOffset(OffsetType offset)
                            {
                                if(offset < Config::pageSize)
                                {
                                    this->offset=offset;
                                    return true;
                                }
                                else
                                    return false;
                            }

                            /** \brief TODO \todo
                            *
                            * \param page TODO \todo
                            */
                            bool setPage(PageType page)
                            {
                                if(page <= flashPageEnd)
                                {
                                    this->page = page;
                                    return true;
                                }
                                else
                                    return false;
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            LinearAddressType getLinearAddress() const
                            {
                                return ((LinearAddressType)page) * Config::pageSize + offset;
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            bool endOfPage() const
                            {
                                return ( offset == Config::pageSize );
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            void next()
                            {
                                offset = offset + sizeof(ContentType);
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            void nextPage()
                            {
                                page++;
                                offset = 0;
                            }
                    };

                    /** \brief TODO \todo
                     *
                     * TODO \todo
                     */
                    struct Writer
                    {
                        private:
                            /** \brief TODO \todo */
                            Address current;

                            /** \brief TODO \todo
                             *
                             * \param offset TODO \todo
                             */
                            void preserveWord(OffsetType offset)
                            {
                                if( Config::preserveUnset )
                                {
                                    Address temp=current;
                                    ContentType buffer;
                                    temp.setOffset(offset);
                                    read(temp.getLinearAddress(), buffer);
                                    writeBuffer(offset, buffer);
                                }
                            }

                        public:
                            /** \brief TODO \todo */
                            static const OffsetType pageSize = Config::pageSize;

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            Address getCurrentAddress() const
                            {
                                return current;
                            }

                            /** \brief TODO \todo
                             *
                             * \param address TODO \todo
                             */
                            bool setCurrentAddress(const LinearAddressType address)
                            {
                                return setCurrentAddress(Address(address));
                            }

                            /** \brief TODO \todo
                             *
                             * \param address TODO \todo
                             */
                            bool setCurrentAddress(const Address address)
                            {
                                if(address.isValid())
                                {
                                    current=address;
                                    for(OffsetType temp = 0; temp < current.getOffset(); temp+=2)
                                        preserveWord(temp);
                                    return true;
                                }
                                else
                                    return false;
                            }

                            /** \brief TODO \todo
                             *
                             * \param data TODO \todo
                             */
                            void write(ContentType data)
                            {
                                writeBuffer(current.getOffset(), data);
                                waitUntilDone();

                                if( Config::autoIncrement )
                                    current.next();

                                if( Config::autoWritePage &&
                                    current.endOfPage() )
                                {
                                    writeCurrentPage();
                                    current.nextPage();
                                }

                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            void eraseCurrentPage()
                            {
                                erasePage(current.getPage());
                                waitUntilDone();
                                enableRWW();
                            }

                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            void writeCurrentPage()
                            {
                                for( Address temp = current ;
                                     ! temp.endOfPage()          ;
                                     temp.next()                 )
                                    preserveWord(temp.getOffset());

                                if( Config::autoErase )
                                {
                                    erasePage(current.getPage());
                                    waitUntilDone();
                                }
                                writePage( current.getPage() );
                                waitUntilDone();
                                enableRWW();
                            }
                    };

                    /** \brief TODO \todo
                     *
                     * TODO \todo
                     */
                    struct Reader
                    {
                        private:
                            /** \brief TODO \todo */
                            Address current;

                        public:
                            /** \brief TODO \todo
                             *
                             * TODO \todo
                             */
                            Address getCurrentAddress() const
                            {
                                return current;
                            }

                            /** \brief TODO \todo
                             *
                             * \param address TODO \todo
                             */
                            bool setCurrentAddress(const LinearAddressType address)
                            {
                                return setCurrentAddress(Address(address));
                            }

                            /** \brief TODO \todo
                             *
                             * \param address TODO \todo
                             */
                            bool setCurrentAddress(const Address address)
                            {
                                if(address.isValid())
                                {
                                    current=address;
                                    return true;
                                }
                                else
                                    return false;
                            }

                            /** \brief TODO \todo
                             *
                             * \param value TODO \todo
                             */
                            void read(ContentType& value)
                            {
                                type::read(current.getLinearAddress(), value);

                                if(Config::autoIncrement)
                                    current.next();
                            }
                    };
            };
        };
    };

    /** \example flash.cpp
    *
    * TODO
    */
}
}
}
