#pragma once
#include "global.h"
#include "settings.h"
#include "ui.h"
#include "words.h"

/**
 * Reveals phase: the device is passed around the table. Each player holds A
 * to fill the pie wheel and reveal their role; they then hold B to pass
 * to the next player.
 */
struct Reveals {
    enum class Sub : uint8_t { Hold, Reveal };

    uint8_t playerCount = 0;
    uint8_t imposterCount = 0;
    uint8_t current = 0; ///< 0-based index of the player taking their turn
    uint8_t wordIndex = 0;
    bool isImposter[MAX_PLAYERS] = {};
    Sub sub = Sub::Hold;

    void begin(uint8_t players, uint8_t imposters) {
        playerCount = players;
        imposterCount = imposters;
        current = 0;
        sub = Sub::Hold;
        btn::resetHold();

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
            case Sub::Hold:   return tickHold();
            case Sub::Reveal: return tickReveal();
        }
        return Phase::Reveals;
    }

private:
    Phase tickHold() {
        // Vertically centered content (size 1 to fit in left 2/3)
        char buf[16];
        snprintf(buf, sizeof(buf), "PLAYER %u", current + 1);
        printCentered(26, buf);
        printCentered(38, "Hold to reveal");

        uint8_t result = btn::tick("REVEAL", nullptr);
        if (result == 1) {
            sub = Sub::Reveal;
            btn::resetHold();
        }
        return Phase::Reveals;
    }

    Phase tickReveal() {
        // Vertically centered content (size 1 to fit in left 2/3)
        if (isImposter[current]) {
            printCentered(26, "YOU ARE THE");
            printCentered(38, "IMPOSTER");
        } else {
            char word[9];
            copyWord(word, wordIndex);
            printCentered(26, "YOUR WORD");
            printCentered(38, word);
        }

        bool last = (current + 1 >= playerCount);

        uint8_t result = btn::tick(nullptr, last ? "START" : "NEXT");
        if (result == 2) {
            current++;
            if (current >= playerCount) {
                return Phase::Play;
            }
            sub = Sub::Hold;
            btn::resetHold();
        }
        return Phase::Reveals;
    }
};
