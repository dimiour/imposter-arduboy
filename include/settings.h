#pragma once
#include "global.h"
#include "ui.h"

// Drum-roll rows, in display order.
enum SettingRow : uint8_t {
    ROW_PLAYERS,
    ROW_IMPOSTERS,
    ROW_START,
    ROW_COUNT,
};

constexpr uint8_t MIN_PLAYERS = 3;
constexpr uint8_t MAX_PLAYERS = 10;

/**
 * Settings phase: a vertical "drum roll" picker.
 *
 * UP / DOWN  : spin the drum to the previous / next row (spring-animated snap).
 * LEFT / RIGHT : change the value of the centered row.
 * A / B (on START) : begin the reveal phase.
 *
 * The scroll is driven by a critically-ish damped spring toward the integer
 * target row, which gives the wheel its smooth, slightly weighty settle.
 * Plain `float` is used here rather than the fixed-point `f16`: it's only a
 * handful of FLOPs per frame on the 16 MHz AVR and keeps the physics readable.
 */
struct Settings {
    static constexpr float STIFFNESS = 0.22f; ///< pull toward target row
    static constexpr float DAMPING = 0.55f;   ///< velocity retained per frame
    static constexpr float KICK = 1.6f;       ///< flick added on each press
    static constexpr int16_t ROW_HEIGHT = 20; ///< px between rows on screen

    float pos = 0; ///< current centered row (fractional)
    float vel = 0; ///< scroll velocity in rows / frame
    int8_t target = 0; ///< row the drum is settling toward

    uint8_t players = 4;
    uint8_t imposters = 1;

    void reset() {
        pos = 0;
        vel = 0;
        target = ROW_PLAYERS;
    }

    Phase tick() {
        Phase next = handleInput();
        physics();
        render();
        return next;
    }

private:
    Phase handleInput() {
        if (arduboy.justPressed(UP_BUTTON) && target > 0) {
            target--;
            vel -= KICK;
        }
        if (arduboy.justPressed(DOWN_BUTTON) && target < ROW_COUNT - 1) {
            target++;
            vel += KICK;
        }

        switch (target) {
            case ROW_PLAYERS:
                if (arduboy.justPressed(RIGHT_BUTTON) && players < MAX_PLAYERS) players++;
                if (arduboy.justPressed(LEFT_BUTTON) && players > MIN_PLAYERS) players--;
                // Imposters must stay strictly below the player count.
                if (imposters > players - 1) imposters = players - 1;
                break;

            case ROW_IMPOSTERS:
                if (arduboy.justPressed(RIGHT_BUTTON) && imposters < players - 1) imposters++;
                if (arduboy.justPressed(LEFT_BUTTON) && imposters > 1) imposters--;
                break;

            case ROW_START:
                if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
                    return Phase::Reveals;
                }
                break;
        }

        return Phase::Settings;
    }

    void physics() {
        vel += (target - pos) * STIFFNESS;
        vel *= DAMPING;
        pos += vel;
    }

    void render() {
        // Selection window framing the centered row.
        arduboy.drawRect(0, 32 - ROW_HEIGHT / 2, WIDTH, ROW_HEIGHT);

        for (uint8_t i = 0; i < ROW_COUNT; i++) {
            int16_t cy = 32 + (int16_t)lroundf((i - pos) * ROW_HEIGHT);
            if (cy < 4 || cy > 60) continue; // off-screen row

            char buf[18];
            bool active = (i == target);

            switch (i) {
                case ROW_PLAYERS:
                    snprintf(buf, sizeof(buf), active ? "< PLAYERS %u >" : "PLAYERS %u", players);
                    break;
                case ROW_IMPOSTERS:
                    snprintf(buf, sizeof(buf), active ? "< IMPOSTERS %u >" : "IMPOSTERS %u", imposters);
                    break;
                case ROW_START:
                    strcpy(buf, active ? ">> START <<" : "START");
                    break;
            }

            printCentered(cy - 4, buf);
        }
    }
};
