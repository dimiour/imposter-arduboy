#include <Arduboy2.h>

ARDUBOY_NO_USB;
Arduboy2 arduboy;

#include "settings.h"
#include "reveals.h"
#include "play.h"

Phase phase = Phase::Settings;

Settings settings;
Reveals reveals;
Play play;

// Initialize a phase as we transition into it.
static void enter(Phase next) {
    switch (next) {
        case Phase::Settings:
            settings.reset();
            break;
        case Phase::Reveals:
            reveals.begin(settings.players, settings.imposters);
            break;
        case Phase::Play:
            play.begin(reveals.playerCount, reveals.imposterCount,
                       reveals.wordIndex, reveals.isImposter);
            break;
    }
    phase = next;
}

void setup() {
    arduboy.boot();
    arduboy.setFrameRate(60);
    arduboy.initRandomSeed();

    btn::init();
    settings.reset();
}

void loop() {
    if (!arduboy.nextFrame()) return;

    arduboy.pollButtons();
    arduboy.clear();

    Phase next = phase;
    switch (phase) {
        case Phase::Settings: next = settings.tick(); break;
        case Phase::Reveals:  next = reveals.tick();  break;
        case Phase::Play:     next = play.tick();     break;
    }

    if (next != phase) enter(next);

    arduboy.display();
}
