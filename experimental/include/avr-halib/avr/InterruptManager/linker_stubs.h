/*******************************************************************************
 *
 * Copyright (c) 2010 Michael Schulze <mschulze@ivs.cs.uni-magdeburg.de>
 * All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions
 *    are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *
 *    * Neither the name of the copyright holders nor the names of
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * $Id$
 *
 ******************************************************************************/

#ifndef __LINKER_STUBS_H_939DA466D2ACD4__
#define __LINKER_STUBS_H_939DA466D2ACD4__

// Satisfying the linker because the following symbol is needed by the usual
// run-time. We define it as naked empty function and align it to address zero
extern "C" void __vector_default () __attribute__((section(".vector0"),used,naked));
extern "C" void __vector_default () {}

// This function is provided to give the vector table a correct name. The nice
// side effect is that the vector table is interpreted by the disassembler as a
// function, which leads to resolve the vector table content as assembler
// mnemonics and/or symbols and we have nice debugging possibilities by using
// e.g. objdump or something like this
extern "C" void __vector_table() __attribute__((section(".vector1"),used,naked));
extern "C" void __vector_table() {
}

// define the reset vector
extern "C" void __reset_vector() __attribute__((section(".vector0"),used,naked));
extern "C" void __reset_vector() {
    asm volatile ("jmp __ctors_end");
}

// defines the target for the reset vector
extern "C" void __ctors_end();

#endif // __LINKER_STUBS_H_939DA466D2ACD4__
