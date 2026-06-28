#pragma once

#include <Arduboy2.h>

extern Arduboy2 arduboy;

/// 16-bit fractional type
/// 0000 0000 0000 0001 * 2^8 = 1 pixel
using f16 = int16_t;

using p8 = int8_t;
