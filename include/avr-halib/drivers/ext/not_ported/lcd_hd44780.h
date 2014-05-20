/** \addtogroup ext */
/*@{*/
/**
 *  \file  include/avr-halib/ext/lcd_hd44780.h
 *  \brief  Defines class LcdHd44780
 *  \author  Karl Fessel, Philipp Werner
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */
#pragma once

#include "avr-halib/common/portmap.h"
#include "avr-halib/common/delay.h"
#include "avr-halib/drivers/ext/hd44780.h"

namespace avr_halib
{
namespace drivers
{
namespace ext
{
    /// Use with COutDevice
    // TODO: Doku!
    template <class LCDPortmap> class LcdHd44780:public Hd44780<LCDPortmap>
    {
        public:
            /// Writes a character to Display
            void put(const char c)
            {
                Hd44780<LCDPortmap>::write(c, true, false);
            }
    };
}
}
}
/*@}*/
