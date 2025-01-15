#include <Arduino.h>
#include "buttons.h"
#include "hardware.h"
#include "leds.h"
#include "motor.h"
#include "play_data.h"
#include "speaker.h"
#include "debug.h"

volatile bool button_states[NUM_BUTTONS];
volatile unsigned long press_time;
bool validated_button_states[NUM_BUTTONS];

// interupt if any button is pressed
// saves the time pressed for debouncing, long press etc
// saves all the button states
void button_pressed_i(){
	press_time = millis();
	button_states[ANY_COLOR_ID] = true;
	button_states[GREEN_ID] = digitalRead(GREEN_BUTTON);
	button_states[AMBER_ID] = digitalRead(AMBER_BUTTON);
	button_states[RED_ID] = digitalRead(RED_BUTTON);
}

// use in conjunction with the ISR
bool button_pressed(){
	// do nothing if no button has been presssed according to the ISR
    if(!button_states[ANY_COLOR_ID])
		return false;

	// enforce a debounce period
    // if the button is unpressed during this time, cancel the press
    // this depends on being called immediately after the ISR has recorded a press,
	//   ie. in a loop

	// save the original pressed button state, that may change in the background
	// if the switch is noisy
	bool saved_button_states[NUM_BUTTONS];
	for(int i = 0; i < NUM_BUTTONS; i++)
		saved_button_states[i] = button_states[i];

    unsigned long debounce_timeout =  press_time + DEBOUNCE_TIME;
	unsigned long drop_count = 0;
	while(millis() < debounce_timeout){
		if(digitalRead(ANY_BUTTON) == LOW)
		    drop_count++;
	}

#ifdef ENABLE_DEBUG_FEATURES
	if(debug_marker == 0)
		set_debug_marker(drop_count);
#endif
	if(drop_count > MAX_DROPS){
		// set_debug_marker(1);
		return false;
	}

	// save the validated button states
	for(int i = 0; i < NUM_BUTTONS; i++)
		validated_button_states[i] = saved_button_states[i];

	// is this necessary?
	button_states[ANY_COLOR_ID] = false;
	return true;
}

bool button_still_pressed(){
	return digitalRead(ANY_BUTTON) == HIGH;
}

// use in conjunction with button_pressed()
// 0=still waiting, 1=long press, -1=canceled
int wait_on_long_press(){
	unsigned long target_time = press_time + LONG_PRESS_TIME;

	if(millis() < target_time){
		// still waiting on long press
		if(button_still_pressed())
			return 0;
		else
			return -1;
	} else {
		// long press time has been met
		if(button_still_pressed()){
			if(option_vibrate)
				vibrate();
			else if(option_sound)
				beep();
			return 1;
		}
		else
			return -1;
	}
}

// returns 0 on long press, -1 if no button pressed, otherwise validated button ID
int handle_long_press(bool show_leds){
	if (button_pressed()) {
		if(show_leds)
			all_leds.activate_leds(button_states, true);
		int long_press_state;
		while ((long_press_state = wait_on_long_press()) == 0)
			;
		if(show_leds)
			all_leds.deactivate_leds(true);
		if (long_press_state == 1){
			return 0;
		}
		else {
			if (validated_button_states[GREEN_ID])
				return GREEN_ID;
			else if (validated_button_states[AMBER_ID])
				return AMBER_ID;
			else if (validated_button_states[RED_ID])
				return RED_ID;
		}
	}
	return -1;
}

void reset_buttons_state(){
	for(int i = 0; i < NUM_BUTTONS; i++)
		button_states[i] = false;
}