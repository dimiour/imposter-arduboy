#pragma once
#include "global.h"

/// The secret word that crewmates see during the reveal phase.
/// Imposters are shown the IMPOSTER screen instead and never learn the word.
///
/// Strings live in flash (PROGMEM) so they don't eat into the ~2.5 KB of SRAM.
/// Keep each word <= 8 characters so it fits at text size 2 (12 px / glyph).

const char word00[] PROGMEM = "PIZZA";
const char word01[] PROGMEM = "BEACH";
const char word02[] PROGMEM = "SCHOOL";
const char word03[] PROGMEM = "DOCTOR";
const char word04[] PROGMEM = "GUITAR";
const char word05[] PROGMEM = "WINTER";
const char word06[] PROGMEM = "ROCKET";
const char word07[] PROGMEM = "CASTLE";
const char word08[] PROGMEM = "ISLAND";
const char word09[] PROGMEM = "BANANA";
const char word10[] PROGMEM = "SOCCER";
const char word11[] PROGMEM = "DRAGON";
const char word12[] PROGMEM = "CAMERA";
const char word13[] PROGMEM = "GARDEN";
const char word14[] PROGMEM = "COFFEE";
const char word15[] PROGMEM = "PIRATE";
const char word16[] PROGMEM = "FOREST";
const char word17[] PROGMEM = "MIRROR";
const char word18[] PROGMEM = "PENGUIN";
const char word19[] PROGMEM = "BUTTER";
const char word20[] PROGMEM = "BUTTER";
const char word21[] PROGMEM = "JUNGLE";
const char word22[] PROGMEM = "PLANET";
const char word23[] PROGMEM = "SUNSET";
const char word24[] PROGMEM = "TUNNEL";
const char word25[] PROGMEM = "VELVET";
const char word26[] PROGMEM = "WIZARD";
const char word27[] PROGMEM = "CANDLE";
const char word28[] PROGMEM = "HELMET";
const char word29[] PROGMEM = "JIGSAW";
const char word30[] PROGMEM = "KITTEN";

const char* const WORDS[] PROGMEM = {
    word00, word01, word02, word03, word04, word05, word06, word07,
    word08, word09, word10, word11, word12, word13, word14, word15,
    word16, word17, word18, word19, word20, word21, word22, word23,
    word24, word25, word26, word27, word28, word29, word30,
};

constexpr uint8_t WORD_COUNT = sizeof(WORDS) / sizeof(WORDS[0]);

/// Copy the word at `index` from flash into the RAM buffer `out`.
/// `out` must be able to hold at least 9 bytes (8 chars + null terminator).
inline void copyWord(char* out, uint8_t index) {
    strcpy_P(out, (PGM_P)pgm_read_ptr(&WORDS[index]));
}
