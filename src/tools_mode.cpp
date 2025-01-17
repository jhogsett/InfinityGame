#include "clock_mode.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "sleep_mode.h"
#include "test_mode.h"
#include "timer_mode.h"
#include "utils.h"
#include "tools_mode.h"
#include "debug.h"

bool tools_menu(){
	switch(button_led_prompt(FSTR("CLOCK    GO "))){
	case -1:
	case 0:
		return false;
	case 3:
		return clock_mode();
	}

	switch(button_led_prompt(FSTR("TIMER    GO "))){
	case -1:
	case 0:
		return false;
	case 3:
		return timer_mode();
	}

	switch(button_led_prompt(FSTR("SLEEP    GO "))){
	case -1:
	case 0:
		return false;
	case 3:
		return sleep_mode();
	}

#ifdef ENABLE_TEST_FEATURES
	switch(button_led_prompt(FSTR("TEST     GO "))){
	case -1:
		return false;
	case 3:
		return test_mode();
	}
#endif

#ifdef ENABLE_DEBUG_FEATURES
	switch(button_led_prompt(FSTR("DEBUG    GO "))){
	case -1:
		return false;
	case 3:
		return debug_mode();
	}
#endif

	switch(button_led_prompt(FSTR("BALANCES GO "))){
	case -1:
	case 0:
		return false;
	case 3:
		return display_balances();
	}

	switch(button_led_prompt(FSTR("SCORES   GO "))){
	case -1:
	case 0:
		return false;
	case 3:
		return display_scores();
	}

	switch(button_led_prompt(FSTR("RESET    GO "))){
	case -1:
		return false;
	case 3:
		return reset_options();
	}

	return false;
}
