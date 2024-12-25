#ifndef __LEDS_H__
#define __LEDS_H__

#include "hardware.h"
#include "led_handler.h"

#define DEFAULT_ALL_LEDS_SHOW_TIME    1000
#define DEFAULT_ALL_LEDS_BLANK_TIME   1000
#define DEFAULT_PANEL_LEDS_SHOW_TIME   750
#define DEFAULT_PANEL_LEDS_BLANK_TIME  350
#define DEFAULT_BUTTON_LEDS_SHOW_TIME  800
#define DEFAULT_BUTTON_LEDS_BLANK_TIME 400
#define TITLE_PANEL_LEDS_SHOW_TIME     100
#define TITLE_PANEL_LEDS_BLANK_TIME    0
#define TITLE_PANEL_LEDS_STYLE (LEDHandler::STYLE_RANDOM)

#define LED_INTENSITY1 32 // Green panel LED is a bit brighter than the Amber and Red LEDs
#define LED_INTENSITY2 40 // Amber panel LED matched to Green
#define LED_INTENSITY3 40 // Red panel LED matched to Red

extern const int led_intensities[];
extern LEDHandler all_leds;
extern LEDHandler panel_leds;
extern LEDHandler button_leds;

#endif
