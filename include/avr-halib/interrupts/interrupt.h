#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptManager.h>

inline void sei() __attribute__((always_inline));
inline void sei()
{
    asm volatile("sei");
}

inline void cli() __attribute__((always_inline));
inline void cli()
{
    asm volatile("cli");
}
