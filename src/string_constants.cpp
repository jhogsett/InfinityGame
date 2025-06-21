#include <Arduino.h>
#include "string_constants.h"

// Common strings used in multiple places - store in PROGMEM
// Define once here to avoid duplication across multiple .cpp files
const char STR_LEDS[] PROGMEM = "LEDS";
const char STR_BEEP[] PROGMEM = "BEEP";
const char STR_BUZZ[] PROGMEM = "BUZZ";
