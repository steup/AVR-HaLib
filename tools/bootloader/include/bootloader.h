#pragma once

#include <interfaces/uart.h>
#include <protocols/avr911.h>
#include <avr-halib/avr/flash.h>
#include <interfaces.h>

namespace avr_halib{
namespace bootloader{
    struct Bootloader{
        struct DefaultConfig{
            typedef communication::Uart::DefaultConfig CommunicationConfig;
            typedef communication::Uart CommunicationDevice;
            typedef protocols::AVR911  ::DefaultConfig ProtocolConfig;
            typedef protocols::AVR911   Protocol;
            typedef drivers::Flash     ::DefaultConfig FlashConfig;
        };

        template<typename Config = DefaultConfig>
        struct configure{

            typedef interface::Commands Commands;

            typedef typename Config::CommunicationConfig CommConf;
            typedef typename Config::ProtocolConfig      ProtoConf;
            typedef typename Config::FlashConfig         FlashConf;

            typedef typename Config::CommunicationDevice::template configure< CommConf >::type CommDevice;
            typedef typename Config::Protocol           ::template configure< ProtoConf >::type Protocol;
            typedef typename drivers::Flash::configure< FlashConf >::type Flash;

            struct type{
                private:
                    void nothing()
                    {
                        //TODO insert functionality
                        this->setResult(true);
                    }
                    void exit()
                    {
                        //TODO insert functionality
                        this->setResult(false);
                    }
                    void signature()
                    {
                        CommandResults::Signature sig;
                        //TODO insert functionality
                        sig.value[0]=0;
                    }
                    void blockSize()
                    {
                        CommandResults::BlockSize size;
                        //TODO insert functionality
                        this->setResult(true, size);
                    }
                    void writeBlock()
                    {
                        CommandParams::WriteBock write;
                        this->getParams(write);
                        //TODO insert functionality
                        this->setResult(true);
                    }
                    void readBlock()
                    {
                        CommandParams::ReadBlock readIn;
                        CommandResults::ReadBlock readOut;
                        this->getParams(readIn);
                        //TODO insert functionality
                        this->setResult(true, readOut);
                    }
                    void startAddress()
                    {
                        CommandParams::StartAddress address;
                        this->getParams(address);
                        //TODO insert functionality
                    }
                    void erase()
                    {
                        //TODO insert functionality
                        this->setResult(true);
                    }
                    void readFuseLock()
                    {
                        CommandParams::ReadFuseLock fuseLock;
                        //TODO insert functionality
                        this->setResult(true, fuseLock);
                    }

                public:
                    void run()
                    {              
                        while(true)
                        {
                            switch(this->getCommand())
                            {
                                case(Commands::nothing):             break;
                                case(Commands::exit):                exit()
                                                                     break;
                                case(Commands::signature):           signature();
                                                                     break;
                                case(Commands::bufferLoadSupport): bufferLoadSupport();
                                                                     break;
                                case(Commands::bufferLoad):          loadBuffer();
                                                                     break;
                                case(Commands::blockRead):           readBlock();
                                                                     break;
                                case(Commands::writeAddess):         setWriteAddress();
                                                                     break;
                                case(Commands::erase):               eraseFlash();
                                                                     break;
                                case(Commands::readFuseLock):        readFuseLock();
                            }
                    }
            };
        };
    };
}
}
