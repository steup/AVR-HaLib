#pragma once

namespace avr_halib{
namespace bootloader{
namespace interfaces{

struct Commands{
    enum CommandType{
        nothing,
        exit,
        signature,
        blockSize,
        writeBlock,
        readBlock,
        startAddress,
        erase,
        readFuseLock
    };
};

typedef Commands::CommandType CommandType;

template<uint16_t bufferSize>
struct CommandParams
{
    enum Target
    {
        flash,
        eeprom
    };

    struct WriteBlock
    {
        uint16_t size;
        Target target;
        uint8_t buffer[bufferSize];
    };

    struct ReadBlock
    {
        uint8_t size;
        Target target;
    };

    struct StartAddress
    {
        uint32_t address;
    };
};

template<uint16_t bufferSize>
struct CommandResults
{
    struct Signature
    {
        uint8_t value[3];
    };
    struct ReadBlock
    {
        uint8_t buffer[bufferSize];
    };
    struct BlockSize
    {
        uint8_t value;
    };
    struct ReadFuseLock
    {
        uint8_t lowFuse;
        uint8_t highFuse;
        uint8_t extFuse;
        uint8_t lock;
    };
}

}
}
}
