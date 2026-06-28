#pragma once

#include <Arduboy2.h>

extern Arduboy2 arduboy;

/// 16-bit fractional type
/// 0000 0000 0000 0001 * 2^8 = 1 pixel
using f16 = int16_t;

using p8 = int8_t;

/// The three high-level phases of the game.
/// Each phase struct exposes a `tick()` that renders itself and
/// returns the phase that should run on the next frame.
enum class Phase : uint8_t {
    Settings, ///< Configure players / imposters (drum roll picker)
    Reveals,  ///< Pass-and-play role reveal (hold-to-fill circle)
    Play,     ///< Discussion + answer reveal
};
