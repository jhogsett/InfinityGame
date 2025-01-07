#include <Arduino.h>
#include "motor.h"
#include "play_data.h"
#include "prompts.h"
#include "speaker.h"
#include "tools_mode.h"
#include "utils.h"
#include "options_mode.h"

bool options_mode(){
	bool buttons[] = {false, true, false, true};
	return branch_prompt(FSTR("TOOLS   SET"), tools_menu, NULL, options_menu, NULL, buttons);
}

bool options_menu(){
	const char *labels[] = {"Off", "On", "12H", "24H", "SLP", "CLK"};
	int result = toggle_prompt(FSTR("SOUND   %s"), labels, option_sound ? 1 : 0, 3, 2) ? true : false;

	if(result == -1)
		return false;
	option_sound = result;
	if(option_sound)
		beep();
	save_data();

	result = toggle_prompt(FSTR("VIBRATE %s"), labels, option_vibrate ? 1 : 0, 3, 2) ? true : false;

	if(result == -1)
		return false;
	option_vibrate = result;
	if(option_vibrate)
		vibrate();
	save_data();

	result = toggle_prompt(FSTR("CLOCK   %s"), labels+2, option_clock_24h ? 1 : 0, 3, 2) ? true : false;

	if(result == -1)
		return false;
	option_clock_24h = result;
	save_data();

	result = toggle_prompt(FSTR("IDLE    %s"), labels+4, option_clock_on_idle ? 1 : 0, 3, 2) ? true : false;

	if(result == -1)
		return false;
	option_clock_on_idle = result;
	save_data();

	return false;
}
