#pragma once
#include "global.h"

// player amount 2 - 10
// imposter amount
// imposter random toggle
// player names toggle
// player 1 name
// player 2 name
// ...
// player 10 name

typedef char label[10];

/**
 * Stores a user-configurable setting's 
 * label and value in the drum roll picker.
 *
 * Label is the display name shown on screen.
 * Value is the current setting value.
 */
struct Row {
    uint8_t value; ///< Setting value (max 9 chars)

    label label; ///< Display name (max 9 chars)

    Row(uint8_t value, const char* label) : value(value), label() {
        strncpy(this->label, label, sizeof(this->label));
    }
};

struct Settings {
    p8 pos; ///< current scroll position
    f16 vel; ///< velocity of scrolling (pos = down) in 64 px / s

    /// TODO: add a clicky effect that gravitates to a value 
    /// so that as t -> infinity; pos -> ( pos % row height ) * pos
    /// where row height is the number of rows in the drum roll picker

    Settings() : pos(0), vel(0) {}
    
    void tick() {
        pos += vel;
        vel *= 0.9f;

        if (vel < 0.03f) {
            vel = 0;
        }
    }
};

