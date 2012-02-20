#pragma once

#include <interfaces/uart.h>
#include <protocols/avr911.h>
#include <flash.h>

namespace avr_halib{
namespace bootloader{
    struct Bootloader{
        struct DefaultConfig{
            typedef interfaces::Uart  ::DefaultConfig InterfaceConfig;
            typedef interfaces::Uart    Interface;
            typedef protocols ::AVR911::DefaultConfig ProtocolConfig;
            typedef protocols ::AVR911  Protocol;
        };

        template<typename Config = DefaultConfig>
        struct configure{
            typedef typename Config::InterfaceConfig IfaceConf;
            typedef typename Config::ProtocolConfig  ProtoConf;
            typedef typename Config::Interface::template configure<IfaceConf>::type Interface;
            typedef typename Config::Protocol ::template configure<ProtoConf>::type Protocol;
            struct type : public Interface, public Protocol, public Flash{
                private:
                    Interface iface;
                    Protocol proto;
                    Flash flash;
                    void print(const char* string, uint8_t len){
                        while(len--)
                            put(*string++);
                    }
                public:
                    void run(){
                        uint8_t buffer;
                        while(true){
                            if(get(buffer)){
                                handleByte(buffer);
                                Protocol::CommandType cmd;
                                if(cmd=getCommand()){
                                    switch(cmd){
                                        default: break;
                                    }
                                    const char* string;
                                    uint8_t len;
                                    if(getCommandResult(string, len))
                                        print(string, len);
                                }
                            }
                        }
                    }
            };
        };
    };
}
}
