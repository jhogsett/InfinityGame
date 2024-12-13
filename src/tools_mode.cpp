#include "clock_mode.h"
#include "play_data.h"
#include "prompts.h"
#include "sleep_mode.h"
#include "test_mode.h"
#include "timer_mode.h"
#include "utils.h"
#include "tools_mode.h"

void tools_menu(){
	switch(button_led_prompt(load_f_string(F("CLOCK   Go  ")))){
	case -1:
		return;
	case 3:
		clock_mode();
		return;
	}

	switch(button_led_prompt(load_f_string(F("TIMER   Go  ")))){
	case -1:
		return;
	case 3:
		timer_mode();
		return;
	}

	switch(button_led_prompt(load_f_string(F("SLEEP   Go  ")))){
	case -1:
		return;
	case 3:
		sleep_mode();
		return;
	}

	switch(button_led_prompt(load_f_string(F("RESET   Go  ")))){
	case -1:
		return;
	case 3:
		reset_options();
		return;
	}

	switch(button_led_prompt(load_f_string(F("TEST    Go  ")))){
	case -1:
		return;
	case 3:
		test_mode();
		return;
	}
}
