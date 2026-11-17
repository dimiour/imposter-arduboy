#pragma once
#include "global.h"

/// Layout constants: left 2/3 for content, right 1/3 for buttons
namespace layout {
    constexpr uint8_t CONTENT_WIDTH = 85;  ///< left 2/3 of 128px screen
    constexpr uint8_t BUTTON_AREA_X = 85;  ///< right 1/3 starts here
    constexpr uint8_t BUTTON_AREA_WIDTH = 43;
}

/// Print `text` centered in the left 2/3 of screen at vertical position `y`.
/// `size` is the Arduboy text scale (1 = 6x8 px glyphs).
inline void printCentered(int16_t y, const char* text, uint8_t size = 1) {
    arduboy.setTextSize(size);

    uint8_t len = strlen(text);
    // Each glyph cell is 6 px wide at size 1; the trailing 1 px gap is dropped.
    int16_t w = (int16_t)len * 6 * size - size;
    int16_t x = (layout::CONTENT_WIDTH - w) / 2;
    if (x < 0) x = 0;

    arduboy.setCursor(x, y);
    arduboy.print(text);
    arduboy.setTextSize(1);
}

/**
 * Unified button UI with pie-wheel hold-to-confirm for all actions.
 * Both buttons are always visible in the right 1/3 of screen.
 * Hold a button to fill its pie; release before full to cancel.
 * Actions only trigger when pie is full.
 * 
 * Buttons are diagonal: B up-right, A down-left.
 * "A" and "B" letters shown inside buttons.
 * Labels appear above B and below A, centered on the button.
 */
namespace btn {
    constexpr uint8_t RADIUS = 7;         ///< button circle radius
    constexpr uint8_t PIE_RADIUS = 5;     ///< pie fill radius (inside button)
    constexpr uint8_t SEGMENTS = 24;      ///< pie segments for full circle
    constexpr uint8_t RETRACT = 2;        ///< how fast pie drains when released

    // Diagonal layout: B up-right, A down-left
    constexpr uint8_t B_X = 114;          ///< B button X (right)
    constexpr uint8_t B_Y = 24;           ///< B button Y (top)
    constexpr uint8_t A_X = 98;           ///< A button X (left)
    constexpr uint8_t A_Y = 36;           ///< A button Y (bottom)
    constexpr uint8_t LABEL_OFFSET = 11;  ///< pixels from button center to label

    // Precomputed unit-circle points for pie drawing
    static int8_t px[SEGMENTS + 1];
    static int8_t py[SEGMENTS + 1];
    static bool initialized = false;

    inline void init() {
        if (initialized) return;
        for (uint8_t i = 0; i <= SEGMENTS; i++) {
            float a = -HALF_PI + (TWO_PI * i) / SEGMENTS;
            px[i] = (int8_t)lroundf(cosf(a) * PIE_RADIUS);
            py[i] = (int8_t)lroundf(sinf(a) * PIE_RADIUS);
        }
        initialized = true;
    }

    /// Draw pie fill at given center
    inline void drawPie(int16_t cx, int16_t cy, uint8_t filled) {
        for (uint8_t i = 0; i < filled && i < SEGMENTS; i++) {
            arduboy.fillTriangle(cx, cy,
                                 cx + px[i], cy + py[i],
                                 cx + px[i + 1], cy + py[i + 1], WHITE);
        }
    }

    /// Draw centered text at given position
    inline void drawLabel(int16_t cx, int16_t y, const char* text) {
        uint8_t len = strlen(text);
        int16_t w = (int16_t)len * 6 - 1;
        int16_t x = cx - w / 2;
        if (x < layout::BUTTON_AREA_X) x = layout::BUTTON_AREA_X;
        if (x + w > WIDTH) x = WIDTH - w;
        arduboy.setCursor(x, y);
        arduboy.print(text);
    }

    /// Draw button letter centered in circle (inverted when pie is filling)
    inline void drawButtonLetter(int16_t cx, int16_t cy, char letter, uint8_t hold) {
        // If pie is filling, letter needs to contrast with white fill
        uint8_t color = (hold > SEGMENTS / 3) ? BLACK : WHITE;
        arduboy.setCursor(cx - 2, cy - 3);
        arduboy.setTextColor(color);
        arduboy.print(letter);
        arduboy.setTextColor(WHITE);
    }

    // Hold state for each button
    static uint8_t holdA = 0;
    static uint8_t holdB = 0;

    /// Reset hold states (call when entering a new phase/subphase)
    inline void resetHold() {
        holdA = 0;
        holdB = 0;
    }

    /**
     * Update and render both buttons. Returns which button (if any) just completed.
     * @param labelA Label for A button (nullptr = inactive, no label shown)
     * @param labelB Label for B button (nullptr = inactive, no label shown)
     * @return 0 = none, 1 = A completed, 2 = B completed
     */
    inline uint8_t tick(const char* labelA, const char* labelB) {
        init();
        uint8_t result = 0;

        bool aActive = (labelA != nullptr);
        bool bActive = (labelB != nullptr);

        // Update A hold state
        if (aActive && arduboy.pressed(A_BUTTON)) {
            if (holdA < SEGMENTS) holdA++;
            if (holdA >= SEGMENTS) result = 1;
        } else if (holdA > 0) {
            holdA = (holdA > RETRACT) ? holdA - RETRACT : 0;
        }

        // Update B hold state
        if (bActive && arduboy.pressed(B_BUTTON)) {
            if (holdB < SEGMENTS) holdB++;
            if (holdB >= SEGMENTS) result = 2;
        } else if (holdB > 0) {
            holdB = (holdB > RETRACT) ? holdB - RETRACT : 0;
        }

        // Draw B button (up-right) with label above if active
        arduboy.drawCircle(B_X, B_Y, RADIUS);
        drawPie(B_X, B_Y, holdB);
        drawButtonLetter(B_X, B_Y, 'B', holdB);
        if (bActive) {
            drawLabel(B_X, B_Y - LABEL_OFFSET - 6, labelB);
        }

        // Draw A button (down-left) with label below if active
        arduboy.drawCircle(A_X, A_Y, RADIUS);
        drawPie(A_X, A_Y, holdA);
        drawButtonLetter(A_X, A_Y, 'A', holdA);
        if (aActive) {
            drawLabel(A_X, A_Y + LABEL_OFFSET, labelA);
        }

        return result;
    }
}
