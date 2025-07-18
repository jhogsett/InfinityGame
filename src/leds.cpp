#include "hardware.h"
#include "led_handler.h"
#include "leds.h"

const int led_intensities[] = {0, 0, 0, LED_INTENSITY1, LED_INTENSITY2, LED_INTENSITY3};
const int led_pins[] = {GREEN_BUTTON_LED, AMBER_BUTTON_LED, RED_BUTTON_LED, GREEN_PANEL_LED, AMBER_PANEL_LED, RED_PANEL_LED};

LEDHandler all_leds(led_pins, 6, led_intensities);
LEDHandler panel_leds(led_pins+3, 3, led_intensities + 3);
LEDHandler button_leds(led_pins, 3, led_intensities);

