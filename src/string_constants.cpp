#include <Arduino.h>
#include "string_constants.h"

// Common strings used in multiple places - store in PROGMEM
// Define once here to avoid duplication across multiple .cpp files
const char STR_CASH[] PROGMEM = "CASH";
const char STR_LOAN[] PROGMEM = "LOAN";
const char STR_HOUSE[] PROGMEM = "HOUSE";
const char STR_GANG[] PROGMEM = "GANG";
const char STR_BANK[] PROGMEM = "BANK";
const char STR_SUM[] PROGMEM = "SUM";
const char STR_LEDS[] PROGMEM = "LEDS";
const char STR_BEEP[] PROGMEM = "BEEP";
const char STR_BUZZ[] PROGMEM = "BUZZ";
const char STR_ALL[] PROGMEM = "ALL";
const char STR_RPT[] PROGMEM = "RPT";

// Money format strings
const char FMT_MONEY[] PROGMEM = "$%s";
const char FMT_CASH_MONEY[] PROGMEM = "CASH $%s";
const char FMT_WIN_MONEY[] PROGMEM = "*WIN $%s";
const char FMT_JACKPOT_MONEY[] PROGMEM = "** JACKPOT $%s";
const char FMT_AUTO_BET[] PROGMEM = "AUTO BET $%s";
const char FMT_BET_BACK[] PROGMEM = "BET %s Back";
const char FMT_LABEL_MONEY[] PROGMEM = "%s $%s";

// Number format strings
const char FMT_WPM[] PROGMEM = "WPM %d";
const char FMT_BEAT_MOVES[] PROGMEM = "BEAT %d MOVES";
const char FMT_BEAT_TIME[] PROGMEM = "Beat %s";
const char FMT_MOVES[] PROGMEM = "  MOVES %d";
const char FMT_ROUNDS[] PROGMEM = "%3d Rounds";
