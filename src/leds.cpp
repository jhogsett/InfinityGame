#include "hardware.h"
#include "led_handler.h"
#include "leds.h"

const int led_intensities[] = {0, 0, 0, LED_INTENSITY1, LED_INTENSITY2, LED_INTENSITY3};
LEDHandler all_leds(FIRST_LED, 6, led_intensities);
LEDHandler panel_leds(FIRST_PANEL_LED, 3, led_intensities + 3);
LEDHandler button_leds(FIRST_BUTTON_LED, 3, led_intensities);
