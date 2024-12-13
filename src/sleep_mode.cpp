#include "displays.h"
#include "leds.h"
#include "prompts.h"
#include "sleep_mode.h"

void sleep_mode(){
	display.clear();
	while(panel_led_prompt() == -1);
}
