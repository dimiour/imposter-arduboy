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
const char word20[] PROGMEM = "JUNGLE";
const char word21[] PROGMEM = "PLANET";
const char word22[] PROGMEM = "SUNSET";
const char word23[] PROGMEM = "TUNNEL";
const char word24[] PROGMEM = "VELVET";
const char word25[] PROGMEM = "WIZARD";
const char word26[] PROGMEM = "CANDLE";
const char word27[] PROGMEM = "HELMET";
const char word28[] PROGMEM = "JIGSAW";
const char word29[] PROGMEM = "KITTEN";
const char word30[] PROGMEM = "LIBRARY";
const char word31[] PROGMEM = "MONSTER";
const char word32[] PROGMEM = "RAINBOW";
const char word33[] PROGMEM = "THUNDER";
const char word34[] PROGMEM = "VAMPIRE";
const char word35[] PROGMEM = "AIRPORT";
const char word36[] PROGMEM = "BALLOON";
const char word37[] PROGMEM = "CHICKEN";
const char word38[] PROGMEM = "DOLPHIN";
const char word39[] PROGMEM = "ELEPHANT";
const char word40[] PROGMEM = "FIREMAN";
const char word41[] PROGMEM = "HOSPITAL";
const char word42[] PROGMEM = "ICEBERG";
const char word43[] PROGMEM = "JEWELRY";
const char word44[] PROGMEM = "KITCHEN";
const char word45[] PROGMEM = "LOBSTER";
const char word46[] PROGMEM = "MOUNTAIN";
const char word47[] PROGMEM = "NOTEBOOK";
const char word48[] PROGMEM = "OCTOPUS";
const char word49[] PROGMEM = "PANCAKE";
const char word50[] PROGMEM = "QUEEN";
const char word51[] PROGMEM = "RABBIT";
const char word52[] PROGMEM = "SANDWICH";
const char word53[] PROGMEM = "TEACHER";
const char word54[] PROGMEM = "UMBRELLA";
const char word55[] PROGMEM = "VOLCANO";
const char word56[] PROGMEM = "WATERFALL";
const char word57[] PROGMEM = "XYLOPHONE";
const char word58[] PROGMEM = "YOGURT";
const char word59[] PROGMEM = "ZEBRA";
const char word60[] PROGMEM = "ANCHOR";
const char word61[] PROGMEM = "BRIDGE";
const char word62[] PROGMEM = "CIRCUS";
const char word63[] PROGMEM = "DESSERT";
const char word64[] PROGMEM = "FARMER";
const char word65[] PROGMEM = "GHOST";
const char word66[] PROGMEM = "HUNTER";
const char word67[] PROGMEM = "INVENTOR";
const char word68[] PROGMEM = "JUGGLER";
const char word69[] PROGMEM = "KING";
const char word70[] PROGMEM = "LAWYER";
const char word71[] PROGMEM = "MAGICIAN";
const char word72[] PROGMEM = "NINJA";
const char word73[] PROGMEM = "OFFICER";
const char word74[] PROGMEM = "PAINTER";
const char word75[] PROGMEM = "ROBOT";
const char word76[] PROGMEM = "SAILOR";
const char word77[] PROGMEM = "TOURIST";
const char word78[] PROGMEM = "UNICORN";
const char word79[] PROGMEM = "VILLAIN";

const char* const WORDS[] PROGMEM = {
    word00, word01, word02, word03, word04, word05, word06, word07,
    word08, word09, word10, word11, word12, word13, word14, word15,
    word16, word17, word18, word19, word20, word21, word22, word23,
    word24, word25, word26, word27, word28, word29, word30, word31,
    word32, word33, word34, word35, word36, word37, word38, word39,
    word40, word41, word42, word43, word44, word45, word46, word47,
    word48, word49, word50, word51, word52, word53, word54, word55,
    word56, word57, word58, word59, word60, word61, word62, word63,
    word64, word65, word66, word67, word68, word69, word70, word71,
    word72, word73, word74, word75, word76, word77, word78, word79,
};

constexpr uint8_t WORD_COUNT = sizeof(WORDS) / sizeof(WORDS[0]);

/// Copy the word at `index` from flash into the RAM buffer `out`.
/// `out` must be able to hold at least 9 bytes (8 chars + null terminator).
inline void copyWord(char* out, uint8_t index) {
    strcpy_P(out, (PGM_P)pgm_read_ptr(&WORDS[index]));
}
