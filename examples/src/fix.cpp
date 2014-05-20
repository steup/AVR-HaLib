#include <platform.h>

#include <avr-halib/common/fixPoint.h>

#include <stdlib.h>
#include <stdint.h>

typedef avr_halib::object::FixPointValue< int16_t, 8 > FixPoint;

static const uint8_t testSize = 100;

FixPoint genValue()
{
  return FixPoint::epsilon() * rand() / (RAND_MAX >> FixPoint::exponent);
}

int main()
{
    for(unsigned int i = 1; i < testSize; i++)
    {
      FixPoint a    = genValue();
      FixPoint b    = genValue();
      FixPoint plus = a + b;
      FixPoint mult = a * b;
      log::emit() << log::setPrecision(4)
          << a    << ", "
          << b    << ", "
          << plus << ", "
          << mult << log::endl;
    }
    return 0;
}
