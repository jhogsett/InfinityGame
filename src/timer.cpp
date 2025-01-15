
#include <Arduino.h>
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "hardware.h"
#include "leds.h"
#include "play_data.h"
#include "speaker.h"
#include "timeouts.h"
#include "utils.h"
#include "timer.h"

byte timer_hour, timer_minute, timer_second;

void render_timer_string(byte seconds, byte minutes, byte hours, bool running) {
	const char *indicator;
	if (running)
		indicator = "STOP";
	else
		indicator = "RUN ";

	if (timer_hour < 1)
		sprintf(display_buffer, FSTR("%s %02d %02d  "), indicator, timer_minute, timer_second);
	else {
		sprintf(display_buffer, FSTR("%s %02d. %02d  "), indicator, timer_hour, timer_minute);
	}
}

void increment_timer(byte &second, byte &minute, byte &hour, byte seconds, byte minutes, byte hours) {
	long total_seconds = time_to_seconds(second, minute, hour);
	total_seconds += time_to_seconds(seconds, minutes, hours);
	seconds_to_time(total_seconds, second, minute, hour);
}

bool decrement_timer(byte &second, byte &minute, byte &hour, int seconds, int minutes, int hours) {
	long total_seconds = time_to_seconds(second, minute, hour);
	total_seconds -= time_to_seconds(seconds, minutes, hours);
	if (total_seconds < 0)
		total_seconds = 0;
	seconds_to_time(total_seconds, second, minute, hour);
	return total_seconds > 0;
}

// TODO timer mode should time out per sleep timeout after no activity
bool timer_prompt(byte seconds, byte minutes, byte hours) {
	unsigned long time = millis();
	unsigned long idle_timeout = time + option_idle_time;

	// unsigned long next_second = millis() + 1000;
	unsigned long next_second = 0;
	timer_hour = hours;
	timer_minute = minutes;
	timer_second = seconds;
	bool running = false;
	bool going_up = false;

	render_timer_string(timer_second, timer_minute, timer_hour, running);
	display.show_string(display_buffer);

	while(true){
		time = millis();
		if(!running && (time >= idle_timeout))
			break;

		if (running && time >= next_second) {
			if (going_up)
				increment_timer(timer_second, timer_minute, timer_hour);
			else if (!decrement_timer(timer_second, timer_minute, timer_hour)){
				running = false;
				idle_timeout = time + option_idle_time;
				render_timer_string(timer_second, timer_minute, timer_hour, running);
				display.show_string(display_buffer);
				if(option_sound)
					alert();
			}
			render_timer_string(timer_second, timer_minute, timer_hour, running);
			display.show_string(display_buffer);
			next_second = time + 1000;
		}

		int button_id;
		if((button_id = handle_long_press()) != -1){
			if(button_id == 0)
				break;
			else {
				idle_timeout = time + option_idle_time;
				switch(button_id){
						case GREEN_ID:
						if (!running) {
							running = true;
							going_up = time_to_seconds(timer_second, timer_minute, timer_hour) == 0;
							next_second = millis() + 1000;
						} else {
							running = false;
						}
						break;
					case AMBER_ID:
						if (timer_hour < 1)
							increment_timer(timer_second, timer_minute, timer_hour, 0, 1, 0);
						else
							increment_timer(timer_second, timer_minute, timer_hour, 0, 0, 1);
						break;
					case RED_ID:
						if (timer_hour < 1)
							increment_timer(timer_second, timer_minute, timer_hour, 1, 0, 0);
						else
							increment_timer(timer_second, timer_minute, timer_hour, 0, 1, 0);
						break;
				}

				render_timer_string(timer_second, timer_minute, timer_hour, running);
				display.show_string(display_buffer);
			}
		}
	}
	return false;
}
