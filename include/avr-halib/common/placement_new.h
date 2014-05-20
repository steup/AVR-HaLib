#pragma once

#include <stdlib.h>

/** \brief placement new implementation
 *
 * \param size the size of the object to be created, ignored
 * \param ptr pointer to the pre-allocated storage to be used for construction
 *
 * Definition of placement new, since avr-gcc does not provide one.
 */
void* operator new(size_t size, void* ptr) __attribute__((weak));

inline void* operator new(size_t size, void* ptr)
{
    return ptr;
};
