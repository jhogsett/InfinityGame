#include <Arduino.h>
#include "bank.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"
#include "time_game.h"
#include "debug.h"

bool time_game(){
	title_prompt(FSTR("The TimeGame"), TITLE_SHOW_TIMES, true);

	int response;
	const bool buttons[] = {false, true, false, true};
	response = button_led_prompt(FSTR(" GO     Back"), buttons);
	if(response == 0 || response == -1 || response == RED_ID)
		return false;

	display.clear();
	delay(ROUND_DELAY);

	unsigned long mean = 0;
	bool fault = false;
	for(byte i = 0; i < ROUNDS; i++){
		button_leds.activate_all(true);
		display.scroll_string(FSTR("Wait 4 FLASH"), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
		delay(ROUND_DELAY);
		display.clear();
		button_leds.activate_all(false);

		int del = random(MIN_DELAY, MAX_DELAY+1);
		delay(del);

		while(digitalRead(ANY_BUTTON) == HIGH){
			// player already pressing button
			// set_debug_marker(1);
			;
		}

		panel_leds.begin_flash(false, 0);
		unsigned long start_time = micros();

		// panel_leds.flash_leds();
		panel_leds.step_flash(millis());

		// if(digitalRead(ANY_BUTTON) == HIGH){
		// 	fault = true;
		// 	break;
		// }

		while(digitalRead(ANY_BUTTON) == HIGH){
#ifdef ENABLE_DEBUG_FEATURES
			// set_debug_marker(1);
#endif
			// button is glitched on by flashing LEDs?
			panel_leds.step_flash(millis());
		}

		while(digitalRead(ANY_BUTTON) == LOW)
			panel_leds.step_flash(millis());

		unsigned long reaction_time = micros() - start_time;
		while(digitalRead(ANY_BUTTON) == HIGH)
			panel_leds.step_flash(millis());

		mean += reaction_time;

		micros_to_ms(copy_buffer, reaction_time);
		sprintf(display_buffer, FSTR("%s ms"), copy_buffer);
		display.scroll_string(display_buffer, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
		delay(ROUND_DELAY);
		display.clear();
	}

	if(fault){
		sprintf(display_buffer, FSTR("FAULT - Button Problem - Try Again"));
		while(button_led_prompt(display_buffer) == -1);
		return false;
	}

	while(button_pressed());

	mean /= ROUNDS;
	micros_to_ms(copy_buffer, mean);
	sprintf(display_buffer, FSTR("SCORE %s ms"), copy_buffer);
	title_prompt(display_buffer, 1, false, ROUND_DELAY);
	// delay(ROUND_DELAY);

	if(mean < best_time){
		best_time = mean;
		micros_to_ms(copy_buffer, mean);
		sprintf(display_buffer, FSTR("NEW BEST * %s ms"), copy_buffer);
		title_prompt(display_buffer, 1, true, ROUND_DELAY);
		// delay(ROUND_DELAY);

		display_win(TIME_WIN);

		add_to_purse(house_payout(TIME_WIN));
		display_purse();

		save_data();

		sprintf(display_buffer, FSTR("NEW BEST * %s ms"), copy_buffer);
	} else {
		micros_to_ms(copy_buffer, best_time);
		sprintf(display_buffer, FSTR("* Best Time %s ms"), copy_buffer);
	}

	// while(button_led_prompt(display_buffer) == -1);
	button_led_prompt(display_buffer);
}
