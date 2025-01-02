#include <Arduino.h>
#include "buttons.h"
#include "hardware.h"
#include "debug.h"

volatile bool button_states[NUM_BUTTONS];
volatile unsigned long press_time;

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

	// rereading these may be redundant
	button_states[GREEN_ID] = digitalRead(GREEN_BUTTON);
	button_states[AMBER_ID] = digitalRead(AMBER_BUTTON);
	button_states[RED_ID] = digitalRead(RED_BUTTON);

	// enforce a debounce period
    // if the button is unpressed during this time, cancel the press
    // this depends on being called immediately after the ISR has recorded a press,
	//   ie. in a loop
    unsigned long debounce_timeout =  press_time + DEBOUNCE_TIME;
	unsigned long drop_count = 0;
	while(millis() < debounce_timeout){
		if(digitalRead(ANY_BUTTON) == LOW)
		    drop_count++;
	}

	if(drop_count > MAX_DROPS){
#ifdef ENABLE_DEBUG_FEATURES
		set_debug_marker(drop_count);
#endif
		return false;
	}

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
		if(button_still_pressed())
			return 1;
		else
			return -1;
	}
}

void reset_buttons_state(){
	for(int i = 0; i < NUM_BUTTONS; i++)
		button_states[i] = false;
}