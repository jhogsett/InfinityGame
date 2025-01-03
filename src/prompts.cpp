
#include <Arduino.h>
#include "billboards_handler.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "play_data.h"
#include "prompts.h"
#include "timeouts.h"
#include "utils.h"

BillboardsHandler billboards_handler(display_buffer, NUM_BILLBOARDS, (const char **)templates, BLANKING_TIME, HOME_TIMES, false, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
char *billboard_data[NUM_BILLBOARDS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void run_billboard(char **data) {
	unsigned long time = millis();
	panel_leds.step(time);
	// button_leds.step(time);
	// all_leds.step(time);
	billboards_handler.run(time, &display, data);
}

// show the billboard and cycle waiting for any button press
void billboard_prompt(voidFuncPtr on_time_out, voidFuncPtr on_press, voidFuncPtr on_long_press) {
	while (button_still_pressed())
		;
	reset_buttons_state();

	unsigned long time = millis();
	unsigned long idle_timeout = time + IDLE_TIMEOUT;

	all_leds.deactivate_leds(true);
	billboards_handler.reset();
	panel_leds.begin(time, LEDHandler::STYLE_RANDOM, 750, 350); // TODO

	// micros_to_ms(display_buffer, best_time);
	// sprintf(copy_buffer, FSTR("Cash $%ld Best Time %s ms"), purse, display_buffer);

	char cash_display[12];
	char time_display[12];

	sprintf(cash_display, FSTR("$%ld"), purse);

	micros_to_ms(copy_buffer, best_time);
	sprintf(time_display, FSTR("%s ms"), copy_buffer);

	billboard_data[BILLBOARD_CASH] = cash_display;
	billboard_data[BILLBOARD_TIME] = time_display;

	while ((time = millis()) < idle_timeout) {
		run_billboard(billboard_data);

		if (button_pressed()) {
			all_leds.activate_leds(button_states, true);
			int long_press_state;

			while ((long_press_state = wait_on_long_press()) == 0)
				;

			all_leds.deactivate_leds(true);

			if (long_press_state == 1) {
				on_long_press();
				return;
			} else {
				on_press();
				return;
			}
		}
	}

	on_time_out();
	return;
}

// prompt with text and cycle waiting for a button response
// returns -1=timed out, 0=long press, button ID otherwise
int button_led_prompt(const char * prompt, const bool *states) {
	unsigned long time;
	unsigned long timeout_time = millis() + PROMPT_TIMEOUT;

	display.begin_scroll_loop();

	// eat an already pressed button on arrival here
	while (((time = millis()) < timeout_time) && button_still_pressed()) {
		display.loop_scroll_string(time, prompt, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
	}
	reset_buttons_state();

	if (states)
		button_leds.activate_leds(states);
	else
		all_leds.deactivate_leds(true);

	while ((time = millis()) < timeout_time) {
		display.loop_scroll_string(time, prompt, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);

		if (button_pressed()) {
			all_leds.activate_leds(button_states, true);
			int long_press_state;
			while ((long_press_state = wait_on_long_press()) == 0)
				;

			all_leds.deactivate_leds(true);
			if (long_press_state == 1){
				return 0;
			}
			else {
				if (button_states[GREEN_ID])
					return GREEN_ID;
				else if (button_states[AMBER_ID])
					return AMBER_ID;
				else if (button_states[RED_ID])
					return RED_ID;
			}
		}
		// restore button leds in case of debounce time failure
		if (states)
			button_leds.activate_leds(states);
	}
	return -1;
}

// prompt with text showing, no cycle waiting for a response
// but cancelable with a button press
// show_panel_leds = true to have them cycle
// show_delay = ensure delay between multiple titles
void title_prompt(const char * title, byte times, bool show_panel_leds, int show_delay) {
	unsigned long time = millis();
	unsigned long timeout_time = time + PROMPT_TIMEOUT;
	unsigned long idle_timeout = time + IDLE_TIMEOUT;

	if (show_panel_leds)
		panel_leds.begin(millis(), TITLE_PANEL_LEDS_STYLE, TITLE_PANEL_LEDS_SHOW_TIME, TITLE_PANEL_LEDS_BLANK_TIME);
	display.begin_scroll_loop(times);

	// eat an already pressed button on arrival here
	while (((time = millis()) < timeout_time) && button_still_pressed()) {
		display.loop_scroll_string(time, title, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);

		if (show_panel_leds)
			panel_leds.step(time);
	}
	reset_buttons_state();

	all_leds.deactivate_leds(true);

	// breaking out of the loop is handled by the display call
	while ((time = millis()) < idle_timeout) {
		if (display.loop_scroll_string(time, title, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME)) {
			if (show_panel_leds)
				panel_leds.step(time);
			if (button_pressed()) {
				while ((wait_on_long_press()) == 0)
					;
				break;
			}
		} else
			break;
	}

	// keep checking for a button press and optionally keep the leds cycling during the show delay
	// the display is no longer scrolled during this period
	if(show_delay){
		unsigned long show_timeout = millis() + show_delay;
		while ((time = millis()) < show_timeout) {
			if (show_panel_leds)
				panel_leds.step(time);
			if (button_pressed()) {
				while ((wait_on_long_press()) == 0)
					;
				break;
			}
		}
	}

	if (show_panel_leds)
		panel_leds.deactivate_leds();
}

// prompt with panel leds showing only and cycle waiting for any button press
int panel_led_prompt() {
	unsigned long time;
	unsigned long timeout_time = millis() + PROMPT_TIMEOUT;

	panel_leds.begin(millis(), LEDHandler::STYLE_RANDOM | LEDHandler::STYLE_BLANKING, 1500, 1500);

	// eat an already pressed button on arrival here
	while (((time = millis()) < timeout_time) && button_still_pressed()) {
		panel_leds.step(time);
	}
	reset_buttons_state();

	all_leds.deactivate_leds(true);

	// the sleep mode never times out
	while (true) {
		time = millis();
		panel_leds.step(time);
		if (!button_pressed())
			continue;

		all_leds.activate_leds(button_states, true);
		while (button_still_pressed())
			;

		all_leds.deactivate_leds(true);
		return 1;
	}

	return -1;
}

// TODO button_led_prompt() blocks, so the loop here might not be needed (would be if there were LEDS or the display to run here)
// returns if timed out waiting for input
void branch_prompt(const char * prompt, voidFuncPtr on_option_1, voidFuncPtr on_option_2, voidFuncPtr on_option_3, voidFuncPtr on_long_press, const bool *states) {
	unsigned long prompt_timeout = millis() + PROMPT_TIMEOUT;
	unsigned long time;

	while ((time = millis()) < prompt_timeout) {
		int choice = button_led_prompt(prompt, states);
		switch (choice) {
			case -1:
				return;
			case 0:
				if (on_long_press)
					on_long_press();
				// always return so long pressing is a way to go back
				return;
			case 1:
				if (on_option_1) {
					on_option_1();
					return;
				}
				break;
			case 2:
				if (on_option_2) {
					on_option_2();
					return;
				}
				break;
			case 3:
				if (on_option_3) {
					on_option_3();
					return;
				}
			break;
		}
	}
}

// returns -1 on timeout or long press, otherwise current choice
// current_choice and return value are zero-based
// toggle_position is one-based to be consistent with button states
int toggle_prompt(const char * prompt, const char **labels, byte current_choice, byte toggle_position, byte num_choices) {
	unsigned long time;
	unsigned long prompt_timeout = millis() + PROMPT_TIMEOUT;

	while ((time = millis()) < prompt_timeout) {
		sprintf(display_buffer, prompt, labels[current_choice]);

		bool states[] = { false, false, false, false };
		states[toggle_position] = true;

		int choice = button_led_prompt(display_buffer, states);
		if (choice == 0 || choice == -1)
			return -1;

		if (choice == toggle_position) {
			current_choice++;
			if (current_choice >= num_choices)
				current_choice = 0;
			displays[toggle_position].scroll_string(labels[current_choice], 1, OPTION_FLIP_SCROLL_TIME);
			continue;
		} else
			return current_choice;
	}
	return -1;
}
