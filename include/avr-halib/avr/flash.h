#pragma once

#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr-halib/avr/interruptLock.h>

namespace avr_halib{
namespace drivers{
    struct Flash
    {
        typedef uint16_t PageOffsetType;
        typedef uint16_t PageIndexType;
        typedef uint16_t PageContentType;
        static const uint32_t farAddressStart = 0x10000;

        struct DefaultConfig
        {
            static const PageOffsetType pageSize     = SPM_PAGESIZE;
            static const PageIndexType  appPageStart = 0;
            static const AddressType    appPageEnd   = 0x1efff / pageSize;
        }

        static uint8_t readByte(uint32_t address)
        {
            if( address >= farAddressStart)
                return pgm_read_byte_far( address );
            else
                return pgm_read_byte_near( address );
        }

        template<typename Config = DefaultConfig>
        struct configure
        {
            struct type
            {
                private:
                    
                    PageIndexType  page;
                    PageOffsetType offset;
                    
                public:
                    Flash() : startAddress(0), pageIndex(0){}

                    bool setPage(PageIndexType page)
                    {
                        if( page < appStartPage || page > appPageEnd )
                            return false;

                        this->page   = page;
                        this->offset = 0;

                        return true;
                    }

                    PageIndexType setPage() const
                    {
                        return page;
                    }

                    bool setOffset(PageOffsetType offset)
                    {
                        if( offset >= Config::pageSize )
                            return false;
                        
                        this->offset = offset;
                    }

                    PageOffsetType getOffset() const
                    {
                        return offset;
                    }   

                    bool writePageBuffer(PageContentType data)
                    {
                        boot_page_fill(page * Config::pageSize + pageIndex, data);
                        pageIndex += 2;
                        return !( pageIndex < Config::pageSize);
                    }

                    uint8_t readByte(PageIndexType page, PageOffsetType offset)
                    {
                        return readByte(page * Config::pageSize + offset);
                    }

                    void erasePage()
                    {
                        GlobaleIntLock();
                        boot_page_erase( page * Config::pageSize );
                        boot_spm_busy_wait();
                        boot_rww_enable;
                        
                    }

                    void writePage()
                    {
                        GlobalIntLock();
                        boot_page_write( page * Config:::pageSize );
                        boot_spm_busy_wait();
                        boot_rww_enable();
                    }
            };
        };
    };
}
}
