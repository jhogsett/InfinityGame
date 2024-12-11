#include "leds.h"
#include "hardware.h"
#include "led_handler.h"

// LEDs handling
#define DEFAULT_ALL_LEDS_SHOW_TIME    1000
#define DEFAULT_ALL_LEDS_BLANK_TIME   1000
#define DEFAULT_PANEL_LEDS_SHOW_TIME   750
#define DEFAULT_PANEL_LEDS_BLANK_TIME  350
#define DEFAULT_BUTTON_LEDS_SHOW_TIME  800
#define DEFAULT_BUTTON_LEDS_BLANK_TIME 400
#define TITLE_PANEL_LEDS_SHOW_TIME     100
#define TITLE_PANEL_LEDS_BLANK_TIME    0
#define TITLE_PANEL_LEDS_STYLE (LEDHandler::STYLE_PLAIN)

const int led_intensities[] = {0, 0, 0, LED_INTENSITY1, LED_INTENSITY2, LED_INTENSITY3};
LEDHandler all_leds(FIRST_LED, 6, led_intensities);
LEDHandler panel_leds(FIRST_PANEL_LED, 3, led_intensities + 3);
LEDHandler button_leds(FIRST_BUTTON_LED, 3, led_intensities);
