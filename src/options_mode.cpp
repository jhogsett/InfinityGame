#include <Arduino.h>
#include "motor.h"
#include "play_data.h"
#include "prompts.h"
#include "speaker.h"
#include "tools_mode.h"
#include "utils.h"
#include "options_mode.h"

#define MINUTE_MILLISECONDS (1000L * 60L)

bool options_mode(){
	bool buttons[] = {false, true, false, true};
	return branch_prompt(FSTR("TOOLS   SET"), tools_menu, NULL, options_menu, NULL, buttons);
}

#define LABELS_OFF_ON 0
#define LABELS_CLOCK 2
#define LABELS_IDLE 4
#define LABELS_IDLE_TO 7

bool options_menu(){ // # optimize strings
	const char *labels[] = {"Off", "On", "12H", "24H", "None", "SLP", "CLK", "5 M", "1 M"};

	int result = toggle_prompt(FSTR("SOUND   %s"), labels + LABELS_OFF_ON, option_sound ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_sound = result == 1 ? true : false;
	if(option_sound)
		beep();
	save_data();

	result = toggle_prompt(FSTR("VIBRATE %s"), labels + LABELS_OFF_ON, option_vibrate ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_vibrate = result == 1 ? true : false;
	if(option_vibrate)
		vibrate();
	save_data();

	result = toggle_prompt(FSTR("CLOCK   %s"), labels + LABELS_CLOCK, option_clock_24h ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_clock_24h = result == 1 ? true : false;
	save_data();

	result = toggle_prompt(FSTR("IDLE    %s"), labels + LABELS_IDLE, option_idle_mode, 3, 3);
	if(result == -1)
		return false;

	option_idle_mode = result;
	save_data();

	int idle_timeout_minutes = option_idle_time / MINUTE_MILLISECONDS;

	result = toggle_prompt(FSTR("IDLE T.O. %s"), labels + LABELS_IDLE_TO, idle_timeout_minutes == 1 ? 1 : 0, 3, 2) ? 1 : 5;

	if(result == -1)
		return false;
	option_idle_time = MINUTE_MILLISECONDS * result;

	save_data();
	return false;
}
