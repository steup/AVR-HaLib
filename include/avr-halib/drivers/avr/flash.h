#pragma once

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr-halib/locking/interruptLock.h>
#include <avr-halib/regmaps/local.h>

namespace avr_halib
{
namespace drivers
{
namespace avr
{
    /** \brief Flash Driver
    *
    */
    struct Flash
    {
        typedef uint16_t OffsetType;
        typedef uint16_t PageType;
        typedef uint16_t ContentType;
        typedef uint32_t LinearAddressType;
        union FuseContentType
        {
            uint32_t value;
            struct{
                uint8_t lowFuses;
                uint8_t highFuses;
                uint8_t extFuses;
            };
        };
        union LockContentType
        {
            uint8_t value;
            struct{
                uint8_t locks;
            };
        };
        static const uint32_t farAddressStart = 0x10000;

        struct DefaultConfig
        {
            typedef regmaps::local::Flash RegMap;


            static const OffsetType        pageSize   = SPM_PAGESIZE;
            static const LinearAddressType flashStart = 0;
            static const LinearAddressType appEnd     = 0x1d000;
            static const LinearAddressType flashEnd   = 0x1ffff;

            static const bool autoIncrement = true;
            static const bool autoWritePage = true;
            static const bool autoErase     = false;
            static const bool preserveUnset = false;
        };

        template<typename Config = DefaultConfig>
        struct configure
        {
            struct type
            {
                private:
                    typedef typename Config::RegMap RegMap;

                public:
                    
                    typedef Config config;

                    static const PageType appPageEnd   = Config::appEnd   / Config::pageSize;
                    static const PageType flashPageEnd = Config::flashEnd / Config::pageSize;

                    static void read(LinearAddressType address, uint8_t& value)
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

                    static void read(LinearAddressType address, ContentType& value)
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

                    static void waitUntilDone()
                    {
                        UseRegMap(rm, RegMap);
                        while(rm.spmen)SyncRegMap(rm);
                        rm.rwwsb=true;
                        SyncRegMap(rm);
                    }

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

                    struct Address
                    {
                        private:
                            PageType   page;
                            OffsetType offset;

                        public:

                            Address() : page(0), offset(0){}
                            Address(const LinearAddressType address)
                            {
                                page   = address / Config::pageSize;
                                offset = address % Config::pageSize;
                            }

                            bool isValid() const
                            {
                                return ( page       <= flashPageEnd     &&
                                         offset     <  Config::pageSize &&
                                         offset % 2 == 0 );
                            }

                            PageType getPage() const
                            {
                                return page;
                            }

                            OffsetType getOffset() const
                            {
                                return offset;
                            }

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


                            LinearAddressType getLinearAddress() const
                            {
                                return ((LinearAddressType)page) * Config::pageSize + offset;
                            }

                            bool endOfPage() const
                            {
                                return ( offset == Config::pageSize );
                            }

                            void next()
                            {
                                offset = offset + sizeof(ContentType);
                            }

                            void nextPage()
                            {
                                page++;
                                offset = 0;
                            }
                    };

                    struct Writer
                    {
                        private:
                            Address current;

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
                            static const OffsetType pageSize = Config::pageSize;

                            Address getCurrentAddress() const
                            {
                                return current;
                            }

                            bool setCurrentAddress(const LinearAddressType address)
                            {
                                return setCurrentAddress(Address(address));
                            }

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

                            void eraseCurrentPage()
                            {
                                erasePage(current.getPage());
                                waitUntilDone();
                                enableRWW();
                            }

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

                    struct Reader
                    {
                        private:
                        
                            Address current;
                        
                        public:
                            Address getCurrentAddress() const
                            {
                                return current;
                            }

                            bool setCurrentAddress(const LinearAddressType address)
                            {
                                return setCurrentAddress(Address(address));
                            }

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

                            void read(ContentType& value)
                            {
                                type::read(current.getLinearAddress(), value);

                                if( Config::autoIncrement )
                                    current.next();
                            }
                    };
            };
        };
    };
}
}
}
