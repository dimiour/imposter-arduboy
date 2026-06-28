#pragma once
#include "global.h"
#include "ui.h"
#include "words.h"

/**
 * Precomputed unit-circle fan used to draw the "fill from the center to the
 * circumference" pie. Points start at 12 o'clock and sweep clockwise. The
 * table is filled once at boot (see `pie::init`) so no trig runs per frame.
 */
namespace pie {
    constexpr uint8_t SEGMENTS = 48; ///< triangles in a full circle
    constexpr uint8_t RADIUS = 22;   ///< px

    static int8_t px[SEGMENTS + 1];
    static int8_t py[SEGMENTS + 1];

    inline void init() {
        for (uint8_t i = 0; i <= SEGMENTS; i++) {
            float a = -HALF_PI + (TWO_PI * i) / SEGMENTS;
            px[i] = (int8_t)lroundf(cosf(a) * RADIUS);
            py[i] = (int8_t)lroundf(sinf(a) * RADIUS);
        }
    }

    /// Draw a pie centered at (cx, cy) filled for the first `filled` segments.
    inline void draw(int16_t cx, int16_t cy, uint8_t filled) {
        for (uint8_t i = 0; i < filled; i++) {
            arduboy.fillTriangle(cx, cy,
                                 cx + px[i], cy + py[i],
                                 cx + px[i + 1], cy + py[i + 1], WHITE);
        }
    }
}

/**
 * Reveals phase: the device is passed around the table. Each player confirms
 * they have it (A), then holds A to sweep-fill a circle. Once the pie is full
 * their secret role is revealed; they must wait a moment then press B to pass
 * to the next player.
 */
struct Reveals {
    static constexpr uint8_t CENTER_X = 64;
    static constexpr uint8_t CENTER_Y = 38;
    static constexpr uint8_t RETRACT = 2; ///< how fast the pie drains when not holding A
    static constexpr uint8_t REVEAL_HOLD_FRAMES = 120; ///< frames (2s @ 60fps) before B is enabled

    enum class Sub : uint8_t { Pass, Hold, Reveal };

    uint8_t playerCount = 0;
    uint8_t imposterCount = 0;
    uint8_t current = 0; ///< 0-based index of the player taking their turn
    uint8_t wordIndex = 0;
    uint8_t holdFrames = 0; ///< 0..pie::SEGMENTS
    uint8_t revealFrames = 0; ///< frames spent in Reveal state; must exceed REVEAL_HOLD_FRAMES
    bool isImposter[MAX_PLAYERS] = {};
    Sub sub = Sub::Pass;

    void begin(uint8_t players, uint8_t imposters) {
        playerCount = players;
        imposterCount = imposters;
        current = 0;
        holdFrames = 0;
        sub = Sub::Pass;

        wordIndex = random(WORD_COUNT);

        for (uint8_t i = 0; i < MAX_PLAYERS; i++) isImposter[i] = false;
        uint8_t assigned = 0;
        while (assigned < imposters) {
            uint8_t r = random(players);
            if (!isImposter[r]) {
                isImposter[r] = true;
                assigned++;
            }
        }
    }

    Phase tick() {
        switch (sub) {
            case Sub::Pass:   return tickPass();
            case Sub::Hold:   return tickHold();
            case Sub::Reveal: return tickReveal();
        }
        return Phase::Reveals;
    }

private:
    Phase tickPass() {
        char buf[12];
        printCentered(8, "PASS DEVICE TO");
        snprintf(buf, sizeof(buf), "PLAYER %u", current + 1);
        printCentered(24, buf, 2);
        printCentered(52, "PRESS (A)");

        if (arduboy.justPressed(A_BUTTON)) {
            holdFrames = 0;
            revealFrames = 0;
            sub = Sub::Hold;
        }
        return Phase::Reveals;
    }

    Phase tickHold() {
        if (arduboy.pressed(A_BUTTON)) {
            if (holdFrames < pie::SEGMENTS) holdFrames++;
            if (holdFrames >= pie::SEGMENTS) sub = Sub::Reveal;
        } else if (holdFrames > 0) {
            holdFrames = (holdFrames > RETRACT) ? holdFrames - RETRACT : 0;
        }

        printCentered(4, "HOLD (A)");
        arduboy.drawCircle(CENTER_X, CENTER_Y, pie::RADIUS + 2);
        pie::draw(CENTER_X, CENTER_Y, holdFrames);
        return Phase::Reveals;
    }

    Phase tickReveal() {
        revealFrames++;

        if (isImposter[current]) {
            printCentered(16, "YOU ARE THE");
            printCentered(30, "IMPOSTER", 2);
        } else {
            char word[9];
            copyWord(word, wordIndex);
            printCentered(10, "YOUR WORD");
            printCentered(26, word, 2);
        }

        bool canPress = (revealFrames >= REVEAL_HOLD_FRAMES);
        bool last = (current + 1 >= playerCount);

        if (canPress) {
            printCentered(54, last ? "(B) START" : "(B) NEXT");
            if (arduboy.justPressed(B_BUTTON)) {
                current++;
                if (current >= playerCount) {
                    return Phase::Play;
                }
                sub = Sub::Pass;
            }
        }
        return Phase::Reveals;
    }
};
