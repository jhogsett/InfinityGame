#include "clock_mode.h"
#include "play_data.h"
#include "prompts.h"
#include "sleep_mode.h"
#include "test_mode.h"
#include "timer_mode.h"
#include "utils.h"
#include "tools_mode.h"
#include "debug.h"

void tools_menu(){
	switch(button_led_prompt(FSTR("CLOCK   Go  "))){
	case -1:
		return;
	case 3:
		clock_mode();
		return;
	}

	switch(button_led_prompt(FSTR("TIMER   Go  "))){
	case -1:
		return;
	case 3:
		timer_mode();
		return;
	}

	switch(button_led_prompt(FSTR("SLEEP   Go  "))){
	case -1:
		return;
	case 3:
		sleep_mode();
		return;
	}

	switch(button_led_prompt(FSTR("RESET   Go  "))){
	case -1:
		return;
	case 3:
		reset_options();
		return;
	}

	switch(button_led_prompt(FSTR("TEST    Go  "))){
	case -1:
		return;
	case 3:
		test_mode();
		return;
	}

	switch(button_led_prompt(FSTR("DEBUG   Go  "))){
	case -1:
		return;
	case 3:
		debug_mode();
		return;
	}
}
