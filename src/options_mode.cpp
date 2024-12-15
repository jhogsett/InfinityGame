#include <Arduino.h>
#include "play_data.h"
#include "prompts.h"
#include "tools_mode.h"
#include "utils.h"
#include "options_mode.h"

void options_mode(){
	bool buttons[] = {false, true, false, true};
	branch_prompt(load_f_string(F("TOOLS    SET")), tools_menu, NULL, options_menu, NULL, buttons);
}

void options_menu(){
	const char *labels[] = {"Off", "On", "12H", "24H", "SLP", "CLK"};
	option_sound = toggle_prompt(load_f_string(F("SOUND   %s")), labels, option_sound ? 1 : 0, 3, 2) ? true : false;
	option_vibrate = toggle_prompt(load_f_string(F("VIBRATE %s")), labels, option_vibrate ? 1 : 0, 3, 2) ? true : false;
	option_clock_24h = toggle_prompt(load_f_string(F("CLOCK   %s")), labels+2, option_clock_24h ? 1 : 0, 3, 2) ? true : false;
	option_clock_on_idle = toggle_prompt(load_f_string(F("IDLE    %s")), labels+4, option_clock_on_idle ? 1 : 0, 3, 2) ? true : false;
	save_data();
}