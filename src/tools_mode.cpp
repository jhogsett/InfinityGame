#include "clock_mode.h"
#include "play_data.h"
#include "prompts.h"
#include "sleep_mode.h"
#include "test_mode.h"
#include "timer_mode.h"
#include "utils.h"
#include "tools_mode.h"
#include "debug.h"

bool tools_menu(){
	switch(button_led_prompt(FSTR("CLOCK   Go  "))){
	case -1:
	case 0:
		return false;
	case 3:
		return clock_mode();
	}

	switch(button_led_prompt(FSTR("TIMER   Go  "))){
	case -1:
	case 0:
		return false;
	case 3:
		return timer_mode();
	}

	switch(button_led_prompt(FSTR("SLEEP   Go  "))){
	case -1:
	case 0:
		return false;
	case 3:
		return sleep_mode();
	}

	switch(button_led_prompt(FSTR("RESET   Go  "))){
	case -1:
		return false;
	case 3:
		return reset_options();
	}

#ifdef ENABLE_TEST_FEATURES
	switch(button_led_prompt(FSTR("TEST    Go  "))){
	case -1:
		return false;
	case 3:
		return test_mode();
	}
#endif

#ifdef ENABLE_DEBUG_FEATURES
	switch(button_led_prompt(FSTR("DEBUG   Go  "))){
	case -1:
		return false;
	case 3:
		return debug_mode();
	}
#endif
	return false;
}
