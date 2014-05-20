#pragma once

#include <interfaces.h>

namespace avr_halib
{
namespace bootloader
{
namespace protocols
{
    struct AVR911
    {
        struct DefaultConfig
        {

        };

        template<typename Config = DefaultConfig>
        struct configure
        {
            struct type : public interfaces::Protocol
            {
                private:
                    char buffer[2];

                public:
                    type()
                    {
                        buffer[1]='\0';
                    }

                    interfaces::CommandType getCommand() const
                    {
                        return interfaces::Commands::nothing;
                    }

                    bool isCommandDone() const
                    {
                        return true;
                    }

                    const char* getCommandResult() const
                    {
                        return buffer;
                    }

                    void handleInput(const uint8_t input)
                    {
                        buffer[0]=input;
                    }
            };
        };
    };
}
}
}
