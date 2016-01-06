#pragma once

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    struct Timer3IntMap
    {
      /** \brief interrupts defined by this device **/
      enum Interrupts
      {
          capture=27, /**< input capture **/
          matchA=28, /**< compare match in unit A **/
          matchB=29, /**< compare match in unit B **/
          matchC=30, /**< compare match in unit C **/
          overflow=31 /**< timer overflow **/
      };
    };
}
}
}
