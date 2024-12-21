#include <Arduino.h>
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "hardware.h"
#include "leds.h"
#include "prompts.h"
#include "utils.h"
#include "test_mode.h"

void test_mode(){
	if(button_led_prompt(FSTR("Press to start GLITCH TEST")) < 1)
		return;

	unsigned long time_out;
	for(int round_num = 1; round_num <= GLITCH_TEST_TIMES; round_num++){
		display.scroll_string(FSTR("HOLD A BUTTON"), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
		while(digitalRead(ANY_BUTTON) == LOW);
		time_out = millis() + GLITCH_TEST_TIMEOUT;
		unsigned long low_count = 0;
		while(millis() < time_out){
		    if(digitalRead(ANY_BUTTON) == LOW)

		low_count++;
		}
		sprintf(display_buffer, FSTR("%d/%d     DONE"), round_num, GLITCH_TEST_TIMES);
		display.scroll_string(display_buffer, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
		while(button_still_pressed());
		reset_buttons_state();
		sprintf(display_buffer, FSTR("%ld DROPS"), low_count);
		title_prompt(display_buffer);
	}


	if(button_led_prompt(FSTR("Press to start 60S NOISE TEST")) < 1)
		return;
	while(button_still_pressed());
	title_prompt(FSTR("Starting....."));

	time_out = millis() + NOISE_TEST_TIMEOUT;

	button_leds.begin(millis(), LEDHandler::STYLE_RANDOM | LEDHandler::STYLE_BLANKING, LED_SHOW_TIME, LED_BLANK_TIME);

	bool states[NUM_BUTTONS] = {false, false, false, false};
	unsigned long time;
	while((time = millis()) < time_out){
		button_leds.step(time);
		if(digitalRead(GREEN_BUTTON) == HIGH)
		    states[GREEN_ID] = true;
		if(digitalRead(AMBER_BUTTON) == HIGH)
		    states[AMBER_ID] = true;
		if(digitalRead(RED_BUTTON) == HIGH)
		    states[RED_ID] = true;
		unsigned long remain = time_out - time;
		unsigned long secs = remain / 1000L;
		unsigned long dec = remain - (secs * 1000L);
		sprintf(display_buffer, FSTR("  %2ld.%04ld"), secs, dec);
		display.show_string(display_buffer, true, false);
		panel_leds.activate_leds(states);
	}
	button_led_prompt(FSTR("Press any button to EXIT"));
}
