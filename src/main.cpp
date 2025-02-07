#include <Arduino.h>
#include <Wire.h>
#include "buttons.h"
#include "displays.h"
#include "hardware.h"
#include "idle_mode.h"
#include "led_handler.h"
#include "leds.h"
#include "morse.h"
#include "motor.h"
#include "options_mode.h"
#include "play_data.h"
#include "prompts.h"
#include "seeding.h"
#include "slot_game.h"
#include "speaker.h"
#include "time_game.h"
#include "utils.h"
#include "word_game.h"

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
	// Serial.begin(115200);
	randomizer.randomize();

	setup_leds();
	setup_display();
	setup_buttons();

	load_save_data();

	attachInterrupt(digitalPinToInterrupt(ANY_BUTTON), button_pressed_i, RISING);
	button_states[ANY_COLOR_ID] = false;

    // send_morse("Infinity Game");

	if(option_sound)
		beep();

	if(option_vibrate)
		vibrate();

}

bool main_menu(){
	bool buttons[] = {false, true, true, true};
	return branch_prompt(FSTR("SlotWordTime"), slots_game, word_game, time_game, NULL, buttons);
}

void loop()
{
	billboard_prompt(idle_mode, main_menu, options_mode);
}
