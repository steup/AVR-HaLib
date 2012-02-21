#pragma once

#include <interfaces/uart.h>
#include <protocols/avr911.h>
#include <avr-halib/avr/flash.h>

namespace avr_halib{
namespace bootloader{
    struct Bootloader{

        struct Commands{
            enum CommandType{
                nothing,
                exit,
                signature,
                bufferLoadSupported,
                bufferLoad,
                blockRead,
                writeAddess,
                erase,
                readLock,
                readLowFuse,
                readHighFuse,
                readExtFuse
            };
        };

        typedef Commdands::CommandType CommandType;

        struct DefaultConfig{
            typedef interfaces::Uart  ::DefaultConfig InterfaceConfig;
            typedef interfaces::Uart    Interface;
            typedef protocols ::AVR911::DefaultConfig ProtocolConfig;
            typedef protocols ::AVR911  Protocol;
            typedef drivers::Flash::DefaultConfig     FlashConfig;
        };

        template<typename Config = DefaultConfig>
        struct configure{

            typedef typename Config::InterfaceConfig IfaceConf;
            typedef typename Config::ProtocolConfig  ProtoConf;
            typedef typename Config::FLashConfig     FlashConf;
            typedef typename Config::Interface::template configure< IfaceConf >::type Interface;
            typedef typename Config::Protocol ::template configure< ProtoConf >::type Protocol;
            typedef typename drivers::Flash::configure< FlashConf >::type Flash;

            struct type{
                private:
                    Interface iface;
                    Protocol proto;
                    Flash flash;

                    uint8_t buffer;

                    void print(const char* string)
                    {
                        while(*string)
                            iface.put((uint8_t)*string++);

                    }
                public:
                    void run()
                    {

                        
                        while(true)
                        {
                            Protocol::CommandType cmd;

                            if(!iface.get(buffer))
                                continue;

                            proto.handleByte(buffer);

                            if(cmd=proto.getCommand())
                            {
                                switch(cmd)
                                {
                                    default: break;
                                }

                                if(proto.getCommandResult())
                                    print(proto.getCommandResult);
                            }
                        }
                    }
            };
        };
    };
}
}
