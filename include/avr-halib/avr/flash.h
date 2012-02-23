#pragma once

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr-halib/avr/interruptLock.h>

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
            static const OffsetType        pageSize   = SPM_PAGESIZE;
            static const LinearAddressType flashStart = 0;
            static const LinearAddressType appEnd     = 0x1d000;
            static const LinearAddressType flashEnd   = 0x1ffff;

            static const bool autoIncrement = true;
            static const bool autoWritePage = false;
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
                    
                        FlashAddress current;

                        void writeBufferWord(OffsetType offset, ContentType word)
                        {
                            
                        }

                        void busyWait()
                        {

                        }

                        void erasePage()
                        {

                        }

                        void writePage()
                        {

                        }

                        void preserveFlashWord(OffsetType offset)
                        {
                            LinearAddressType temp = ((LinearAddressType)current.getPage()) * pageSize + offset;
                            uint16_t buffer = Flash::readWord(temp);
                            boot_page_fill(temp, buffer);
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
                            boot_page_fill(current.getLinearAddress(), data);

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
                            boot_page_erase( current.getLinearAddress() );
                            boot_spm_busy_wait();
                            boot_rww_enable();
                        }

                        void writeCurrentPage()
                        {
                            for( FlashAddress temp = current ;
                                 ! temp.endOfPage()          ;
                                 temp.next()                 )
                                preserveFlashWord(temp.getOffset());

                            if( Config::autoErase )
                            {
                                boot_page_erase( current.getLinearAddress() );
                                boot_spm_busy_wait();
                            }
                            boot_page_write( current.getLinearAddress() );
                            boot_spm_busy_wait();
                            boot_rww_enable();
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
