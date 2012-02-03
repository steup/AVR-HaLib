/*******************************************************************************
 *
 * Copyright (c) 2010-2011 Michael Schulze <mschulze@ivs.cs.uni-magdeburg.de>
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
 * $Id: OpcodeJmp.h 755 2011-12-06 23:14:21Z mschulze $
 *
 ******************************************************************************/

#ifndef __OPCODEJMP_H_F4BB3E01FAF57D__
#define __OPCODEJMP_H_F4BB3E01FAF57D__

namespace Interrupt {

/*! \brief jmp opcode and its target
 *
 * \tparam target has to be a class type containing a target for the jmp and
 *         usually the target points to a function meening it represents a
 *         funtion pointer
 */
template <typename target>
struct OpcodeJmp {
    typedef OpcodeJmp type;

    static void create() __attribute__((always_inline, used)){
        asm volatile (
            "jmp %c[Function]     \n"
            :
            : [Function] "i" (target::target)
        );
    }
};

} /* namespace Interrupt */

#endif // __OPCODEJMP_H_F4BB3E01FAF57D__

