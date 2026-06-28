#pragma once
#include "global.h"
#include "settings.h"
#include "ui.h"
#include "words.h"

/**
 * Play phase: everyone has their role, so now they talk it out.
 *
 * A running clock is shown to time the discussion. Pressing A reveals the
 * answer (the secret word and which players were imposters); pressing B starts
 * a fresh round back at the settings picker.
 */
struct Play {
    uint8_t playerCount = 0;
    uint8_t imposterCount = 0;
    uint8_t wordIndex = 0;
    bool isImposter[MAX_PLAYERS] = {};

    bool answerShown = false;
    uint16_t frames = 0;

    void begin(uint8_t players, uint8_t imposters, uint8_t word, const bool* imposterFlags) {
        playerCount = players;
        imposterCount = imposters;
        wordIndex = word;
        answerShown = false;
        frames = 0;
        for (uint8_t i = 0; i < MAX_PLAYERS; i++) isImposter[i] = imposterFlags[i];
    }

    Phase tick() {
        frames++;

        if (arduboy.justPressed(A_BUTTON)) answerShown = true;
        if (arduboy.justPressed(B_BUTTON)) return Phase::Settings;

        answerShown ? renderAnswer() : renderDiscuss();
        return Phase::Play;
    }

private:
    void renderDiscuss() {
        printCentered(6, "DISCUSS", 2);
        printCentered(26, "Who is faking?");

        // Discussion clock (frames @ 60 fps -> mm:ss).
        uint16_t secs = frames / 60;
        char buf[8];
        snprintf(buf, sizeof(buf), "%u:%02u", secs / 60, secs % 60);
        printCentered(38, buf);

        printCentered(54, "(A) REVEAL  (B) NEW");
    }

    void renderAnswer() {
        char word[9];
        copyWord(word, wordIndex);
        printCentered(2, "THE WORD WAS");
        printCentered(14, word, 2);

        // List the imposter player numbers, e.g. "IMPOSTERS: 2 5".
        char buf[22];
        int n = snprintf(buf, sizeof(buf), "IMPOSTER%s:", imposterCount > 1 ? "S" : "");
        for (uint8_t i = 0; i < playerCount && n < (int)sizeof(buf); i++) {
            if (isImposter[i]) {
                n += snprintf(buf + n, sizeof(buf) - n, " %u", i + 1);
            }
        }
        printCentered(36, buf);

        printCentered(54, "(B) NEW ROUND");
    }
};
