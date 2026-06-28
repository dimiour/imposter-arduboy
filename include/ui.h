#pragma once
#include "global.h"

/// Print `text` horizontally centered on the screen at vertical position `y`.
/// `size` is the Arduboy text scale (1 = 6x8 px glyphs).
inline void printCentered(int16_t y, const char* text, uint8_t size = 1) {
    arduboy.setTextSize(size);

    uint8_t len = strlen(text);
    // Each glyph cell is 6 px wide at size 1; the trailing 1 px gap is dropped.
    int16_t w = (int16_t)len * 6 * size - size;
    int16_t x = (WIDTH - w) / 2;
    if (x < 0) x = 0;

    arduboy.setCursor(x, y);
    arduboy.print(text);
    arduboy.setTextSize(1);
}
