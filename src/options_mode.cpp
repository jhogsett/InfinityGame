#include <Arduino.h>
#include "buffers.h"
#include "motor.h"
#include "play_data.h"
#include "play_views.h"
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

bool options_menu(){ // # optimize strings
	const char *labels_on_off[] = {"Off", "On"};
	int result = toggle_prompt(FSTR("SOUND   %s"), labels_on_off, option_sound ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_sound = result == 1 ? true : false;
	if(option_sound)
		beep();
	save_data();

	result = toggle_prompt(FSTR("VIBRATE %s"), labels_on_off, option_vibrate ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_vibrate = result == 1 ? true : false;
	if(option_vibrate)
		vibrate();
	save_data();

	const char *labels_hi_lo[] = {"LO", "HI"};
	result = toggle_prompt(FSTR("Vib. Str. %s"), labels_hi_lo, option_vib_str ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_vib_str = result == 1 ? true : false;
	if(option_vibrate)
		vibrate();
	save_data();

	const char *labels_clock[] = {"12H", "24H"};
	result = toggle_prompt(FSTR("CLOCK   %s"), labels_clock, option_clock_24h ? 1 : 0, 3, 2);
	if(result == -1)
		return false;

	option_clock_24h = result == 1 ? true : false;
	save_data();

	const char *labels_idle[] = {"None", "SLP", "CLK"};
	result = toggle_prompt(FSTR("IDLE    %s"), labels_idle, option_idle_mode, 3, 3);
	if(result == -1)
		return false;

	option_idle_mode = result;
	save_data();

	int idle_timeout_minutes = option_idle_time / MINUTE_MILLISECONDS;

	const char *labels_idle_to[] = {"5 M", "1 M"};
	result = toggle_prompt(FSTR("IDLE T.O. %s"), labels_idle_to, idle_timeout_minutes == 1 ? 1 : 0, 3, 2) ? 1 : 5;

	if(result == -1)
		return false;
	option_idle_time = MINUTE_MILLISECONDS * result;

	save_data();

	const char *labels_chime[] = {"None", "BEEP", "HOUR", "CODE"};
	result = toggle_prompt(FSTR("CHIME   %s"), labels_chime, option_clock_chime, 3, 4);
	if(result == -1)
		return false;

	option_clock_chime = result;
	save_data();

    option_wpm = prompt_wpm(option_wpm);

    sprintf(copy_buffer, FSTR("%d WPM"), option_wpm);
    send_morse(copy_buffer);

	save_data();

    // ##DATA add settings for new options above here

	return false;
}
