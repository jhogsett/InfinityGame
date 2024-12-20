#include <Arduino.h>
#include <Wire.h>
#include <random_seed.h>
#include "buttons.h"
#include "displays.h"
#include "hardware.h"
#include "idle_mode.h"
#include "led_handler.h"
#include "leds.h"
#include "options_mode.h"
#include "play_data.h"
#include "prompts.h"
#include "slot_game.h"
#include "time_game.h"
#include "utils.h"

#define RANDOM_SEED_PIN A1
static RandomSeed<RANDOM_SEED_PIN> randomizer;

void setup_display(){
	Wire.begin();
	display.init(display_brightnesses);
	/* the duplicated displays do not need reinitialization
	disp1.init(brightness+0);
	disp2.init(brightness+1);
	disp3.init(brightness+2); */
	display.clear();
}

void setup_leds(){
	for(byte i = FIRST_LED; i <= LAST_LED; i++){
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
	unsigned long time = millis();
	panel_leds.begin(time, LEDHandler::STYLE_RANDOM, DEFAULT_PANEL_LEDS_SHOW_TIME, DEFAULT_PANEL_LEDS_BLANK_TIME);
	button_leds.begin(time, LEDHandler::STYLE_BLANKING, DEFAULT_BUTTON_LEDS_SHOW_TIME, DEFAULT_BUTTON_LEDS_BLANK_TIME);
	all_leds.begin(time, LEDHandler::STYLE_RANDOM | LEDHandler::STYLE_BLANKING | LEDHandler::STYLE_MIRROR, DEFAULT_ALL_LEDS_SHOW_TIME, DEFAULT_ALL_LEDS_BLANK_TIME);
}

void setup_buttons(){
	for(byte i = 0; i < NUM_BUTTONS; i++){
		pinMode(i + FIRST_BUTTON, 0x03); // INPUT_PULLDOWN ?!
		button_states[i] = false;
	}
}

void setup(){
	randomizer.randomize();

	setup_leds();
	setup_display();
	setup_buttons();

	load_save_data();

	attachInterrupt(digitalPinToInterrupt(ANY_BUTTON), button_pressed_i, RISING);
	button_states[ANY_COLOR_ID] = false;
}

void main_menu(){
	bool buttons[] = {false, true, false, true};
	branch_prompt(load_f_string(F("Slot    Time")), slots_game, NULL, time_game, NULL, buttons);
}

void loop()
{
	while(button_pressed());
	billboard_prompt(idle_mode, main_menu, options_mode);
}
