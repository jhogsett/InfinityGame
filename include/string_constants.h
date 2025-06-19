#ifndef __STRING_CONSTANTS_H__
#define __STRING_CONSTANTS_H__

#include <Arduino.h>

// Common strings used in multiple places - extern declarations
// Actual definitions are in string_constants.cpp to avoid duplication
extern const char STR_CASH[] PROGMEM;
extern const char STR_LOAN[] PROGMEM;
extern const char STR_HOUSE[] PROGMEM;
extern const char STR_GANG[] PROGMEM;
extern const char STR_BANK[] PROGMEM;
extern const char STR_SUM[] PROGMEM;
extern const char STR_LEDS[] PROGMEM;
extern const char STR_BEEP[] PROGMEM;
extern const char STR_BUZZ[] PROGMEM;
extern const char STR_ALL[] PROGMEM;
extern const char STR_RPT[] PROGMEM;

// Money format strings
extern const char FMT_MONEY[] PROGMEM;
extern const char FMT_CASH_MONEY[] PROGMEM;
extern const char FMT_WIN_MONEY[] PROGMEM;
extern const char FMT_JACKPOT_MONEY[] PROGMEM;
extern const char FMT_AUTO_BET[] PROGMEM;
extern const char FMT_BET_BACK[] PROGMEM;
extern const char FMT_LABEL_MONEY[] PROGMEM;

// Number format strings
extern const char FMT_WPM[] PROGMEM;
extern const char FMT_BEAT_MOVES[] PROGMEM;
extern const char FMT_BEAT_TIME[] PROGMEM;
extern const char FMT_MOVES[] PROGMEM;
extern const char FMT_ROUNDS[] PROGMEM;

#endif
