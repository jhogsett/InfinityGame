#ifndef __LEDS_H__
#define __LEDS_H__

#include "hardware.h"
#include "led_handler.h"

// default on/off time when animating all LEDs
#define DEFAULT_ALL_LEDS_SHOW_TIME    1000
#define DEFAULT_ALL_LEDS_BLANK_TIME   1000

// default on/off time when animating panel LEDs
#define DEFAULT_PANEL_LEDS_SHOW_TIME   750
#define DEFAULT_PANEL_LEDS_BLANK_TIME  350

// default on/off time when animated button LEDs
#define DEFAULT_BUTTON_LEDS_SHOW_TIME  800
#define DEFAULT_BUTTON_LEDS_BLANK_TIME 400

// when dislaying the main billboard prompt, this is the
// timing and style for the panel LEDs
#define BILLBOARD_PANEL_LEDS_SHOW_TIME 750
#define BILLBOARD_PANEL_LEDS_BLANK_TIME 350
#define BILLBOARD_PANEL_LEDS_STYLE (LEDHandler::STYLE_RANDOM)

// when displaying a title with LEDs, this is the on/off time
// this is used for celebrating wins
#define TITLE_PANEL_LEDS_SHOW_TIME     100
#define TITLE_PANEL_LEDS_BLANK_TIME    0
#define TITLE_PANEL_LEDS_STYLE (LEDHandler::STYLE_RANDOM)

// alternate timing to show the leds in sequence
// without blanking at a slower pace
#define TITLE_PANEL_LEDS_SHOW_TIME2     550
#define TITLE_PANEL_LEDS_BLANK_TIME2    0
#define TITLE_PANEL_LEDS_STYLE2 (LEDHandler::STYLE_PLAIN)

// adjustments to match the panels LEDs to the display
#define LED_INTENSITY1 32 // Green panel LED is a bit brighter than the Amber and Red LEDs
#define LED_INTENSITY2 40 // Amber panel LED matched to Green
#define LED_INTENSITY3 40 // Red panel LED matched to Red

// array used to pass the LED intensities to LED handlers
extern const int led_intensities[];

// LED handler for using all 6 LEDs at once
extern LEDHandler all_leds;

// LED handler for just the panel LEDs
extern LEDHandler panel_leds;

// LED handler for just the button LEDs
extern LEDHandler button_leds;

#endif
