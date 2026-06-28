#include <Arduboy2.h>

ARDUBOY_NO_USB;
Arduboy2 arduboy;

#include "settings.h"

enum GameState {
  STATE_MENU,
  STATE_REVEALS,
  STATE_PAUSED,
  STATE_GAME_OVER,
};

GameState state = STATE_MENU;
Settings settings;

void setup() {
  arduboy.boot();
  arduboy.setFrameRate(60);

  settings.init();
}

void loop() {
  if (!arduboy.nextFrame()) return;

  arduboy.pollButtons();
  arduboy.clear();

  switch (state) {
    case STATE_MENU:
        settings.update();
      break;
    case STATE_REVEALS:

      break;
    case STATE_PAUSED:

      break;
    case STATE_GAME_OVER:

      break;
  }
  // do the code
  arduboy.print(F("Hello, world!"));


  arduboy.display();
}
