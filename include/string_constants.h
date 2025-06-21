#ifndef __STRING_CONSTANTS_H__
#define __STRING_CONSTANTS_H__

#include <Arduino.h>

// Common strings used in multiple places - extern declarations
// Actual definitions are in string_constants.cpp to avoid duplication
extern const char STR_LEDS[] PROGMEM;
extern const char STR_BEEP[] PROGMEM;
extern const char STR_BUZZ[] PROGMEM;

#endif
