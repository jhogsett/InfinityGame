#ifndef __LEDS_H__
#define __LEDS_H__

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

static const byte led_intensities[] = {0, 0, 0, 32, 40, 40};
LEDHandler all_leds(FIRST_LED, 6, led_intensities);
LEDHandler panel_leds(FIRST_PANEL_LED, 3, led_intensities + 3);
LEDHandler button_leds(FIRST_BUTTON_LED, 3, led_intensities);

#endif
