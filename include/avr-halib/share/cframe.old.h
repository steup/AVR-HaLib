#pragma once

#warning using this is not recomended please use cdeviceframe.h directly

namespace cdeviceframejail
{
#include "avr-halib/share/cdeviceframe.h"
}
/*! \brief Modifiers of the <code>CFrame</code>*/
struct CFrameModifier:public CFrameModifierBase
{
	enum {esc = 'e', sofr = 'a', eofr = 's', escmod = 0x01};
};

struct CFrameModifierReadable:public CFrameModifierBase
{
	enum {esc = 'e', sofr = 'a', eofr = 's', escmod = 0x20 };
	/*this escmod changes upper case to lower case and vice versa its easier to read*/
};

/*! \class  CFrame CFrame.h "avr-halib/share/CFrame.h"
 *  \brief  This class realizes a bit stuffing by implementing a micro layer.
 *
 *  \tparam character device the CFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam frame modifier used see CFrameModifier
 *  \tparam usable payload size
 */

template < class BaseCDevice, class FLT = uint8_t, class CFM = struct CFrameModifier, FLT PL = 255 >
class CFrameNoInt: public cdeviceframejail::CDeviceFrameNoInt<BaseCDevice, FLT, PL, cdeviceframejail::CFrame< CFM > >
{};

/*! \class  CFrame CFrame.h "avr-halib/share/CFrame.h"
 *  \brief  This class realizes a bit stuffing by implementing a micro layer.
 *
 *  \tparam character device the CFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam frame modifier used see CFrameModifier
 *  \tparam usable payload size
 */
template < class BaseCDevice, class FLT = uint8_t, class CFM = struct CFrameModifier, FLT PL = 255 >
class CFrame: public cdeviceframejail::CDeviceFrame<BaseCDevice, FLT, PL, cdeviceframejail::CFrame< CFM > >
{};
