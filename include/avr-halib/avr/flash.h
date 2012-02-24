#pragma once

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr-halib/avr/interruptLock.h>
#include <avr-halib/regmaps/local.h>

namespace avr_halib{
namespace drivers{
    struct Flash
    {
        typedef uint16_t OffsetType;
        typedef uint16_t PageType;
        typedef uint16_t ContentType;
        typedef uint32_t LinearAddressType;
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
        };

        static uint8_t readByte(LinearAddressType address)
        {
            if( address >= farAddressStart)
                return pgm_read_byte_far( address );
            else
                return pgm_read_byte_near( address );
        }

        static uint16_t readWord(LinearAddressType address)
        {
            if( address >= farAddressStart)
                return pgm_read_word_far( address );
            else
                return pgm_read_word_near( address );
        }

        template<typename Config = DefaultConfig>
        struct configure
        {
            struct type
            {
                struct FlashAddress
                {
                    private:
                        PageType   page;
                        OffsetType offset;

                    public:

                        static const PageType appPageEnd   = Config::appEnd   / Config::pageSize;
                        static const PageType flashPageEnd = Config::flashEnd / Config::pageSize;

                        FlashAddress() : page(0), offset(0){}
                        FlashAddress(const LinearAddressType address)
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

                struct FlashWriter
                {
                    private:
                    
                        typedef typename Config::RegMap RegMap;

                        FlashAddress current;

                        static void writeBuffer(OffsetType offset, ContentType data)
                        {
                            UseRegMap(rm, RegMap);
                            asm volatile( "movw  r0, %2\n\t"
                                          "sts %0, %1\n\t"
                                          "spm\n\t"
                                          "clr  r1\n\t"
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
                        }

                        static void enableRWW()
                        {
                            UseRegMap(rm, RegMap);
                            asm volatile( "sts %0, %1\n\t"
                                          "spm\n\t"
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
                                          "spm\n\t"
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
                                          "spm\n\t"
                                          :
                                          : "i" ((uint16_t)&rm.spmcsr),
                                            "i" ((uint16_t)&rm.rampz),
                                            "r" ((uint8_t)(RegMap::Commands::pageWrite)),
                                            "r" ((uint8_t)(pageExt>>16)&0xff),
                                            "z" ((uint16_t)(pageExt))
                                    );
                        }

                        void preserveFlashWord(OffsetType offset)
                        {
                            FlashAddress temp=current;
                            temp.setOffset(offset);
                            writeBuffer(offset, Flash::readWord(temp.getLinearAddress()));
                        }
                    
                    public:
                        static const OffsetType pageSize = Config::pageSize;

                        FlashAddress getCurrentAddress() const
                        {
                            return current;
                        }

                        bool setCurrentAddress(const LinearAddressType address)
                        {
                            return setCurrentAddress(FlashAddress(address));
                        }

                        bool setCurrentAddress(const FlashAddress address)
                        {
                            if(address.isValid())
                            {
                                current=address;
                                for(OffsetType temp = 0; temp < current.getOffset(); temp+=2)
                                    preserveFlashWord(temp);
                                return true;
                            }
                            else
                                return false;
                        }

                        void write(ContentType data)
                        {
                            writeBuffer(current.getOffset(), data);

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
                            for( FlashAddress temp = current ;
                                 ! temp.endOfPage()          ;
                                 temp.next()                 )
                                preserveFlashWord(temp.getOffset());

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

                struct FlashReader
                {
                    private:
                    
                        FlashAddress current;
                    
                    public:
                        FlashAddress getCurrentAddress() const
                        {
                            return current;
                        }

                        bool setCurrentAddress(const LinearAddressType address)
                        {
                            return setCurrentAddress(FlashAddress(address));
                        }

                        bool setCurrentAddress(const FlashAddress address)
                        {
                            if(address.isValid())
                            {
                                current=address;
                                return true;
                            }
                            else
                                return false;
                        }

                        ContentType read()
                        {
                            ContentType buffer = Flash::readWord(current.getLinearAddress());

                            if( Config::autoIncrement )
                                current.next();

                            return buffer;
                        }
                };
            };
        };
    };
}
}
