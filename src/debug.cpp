#include <Arduino.h>
#include "buffers.h"
#include "displays.h"
#include "prompts.h"
#include "utils.h"
#include "debug.h"

unsigned long debug_marker = 0;

void set_debug_marker(unsigned long marker){
	debug_marker = marker;
}

void clear_debug_marker(){
	debug_marker = 0;
}

void debug_mode(){
	sprintf(display_buffer, FSTR("Debug Marker %lu"), debug_marker);
	button_led_prompt(display_buffer);
	clear_debug_marker();
}
