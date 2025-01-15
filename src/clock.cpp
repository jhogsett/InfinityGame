#include <Arduino.h>
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "play_data.h"
#include "timeouts.h"
#include "utils.h"
#include "clock.h"

unsigned long clock_basis = 0L;
unsigned long time_basis = DEFAULT_TIME_BASIS;
byte clock_hour, clock_minute, clock_second;

void establish_clock_basis(byte seconds, byte minutes, byte hours) {
	clock_basis = millis();
	long total_seconds = time_to_seconds(seconds, minutes, hours);
	time_basis = total_seconds * 1000L;
}

long time_in_seconds() {
	unsigned long time_in_ms = millis() - clock_basis + time_basis;
	if (time_in_ms > MAX_MS_PER_DAY) {
		time_in_ms -= MAX_MS_PER_DAY;
		clock_basis += MAX_MS_PER_DAY;
	}
	return time_in_ms / 1000L;
}

void increment_time_basis(byte &second, byte &minute, byte &hour, byte seconds, byte minutes, byte hours) {
	long total_seconds = time_basis / 1000L;
	total_seconds += time_to_seconds(seconds, minutes, hours);
	seconds_to_time(total_seconds, second, minute, hour);

	if (option_clock_24h) {
	} else {
		if (hour == 0)
			hour = 12;
	}

	total_seconds = time_to_seconds(second, minute, hour);
	time_basis = total_seconds * 1000L;
}

void render_clock_string(byte seconds, byte minutes, byte hours) {
	byte effective_hours;
	if (option_clock_24h) {
		effective_hours = hours;
	} else {
		effective_hours = (hours % 12);
		if (effective_hours == 0)
			effective_hours = 12;
	}

	sprintf(display_buffer, FSTR("  %2d %02d %02d  "), effective_hours, minutes, seconds);
}

// returns true if the clock timed out while being set
	bool clock_prompt(byte seconds, byte minutes, byte hours, byte settable) {
	clock_hour = hours;
	clock_minute = minutes;
	clock_second = seconds;

	render_clock_string(clock_second, clock_minute, clock_hour);
	display.show_string(display_buffer);

	display.begin_scroll_loop();

	unsigned long time = millis();
	unsigned long idle_timeout = time + option_idle_time;

	// clock mode only times out if clock is the idle mode
	while (true) {
		if(settable)
			if((time = millis()) >= idle_timeout)
				return true;

		seconds_to_time(time_in_seconds(), clock_second, clock_minute, clock_hour);

		render_clock_string(clock_second, clock_minute, clock_hour);
		display.show_string(display_buffer);

		// int button_id;
		// if((button_id = handle_long_press()) == 0)
		// 	return false;


		int button_id;
		if((button_id = handle_long_press()) != -1){
			if(button_id == 0)
				return false;
			else {
				if (settable) {
					idle_timeout = time + option_idle_time;

					switch(button_id){
						case GREEN_ID:
							increment_time_basis(clock_second, clock_minute, clock_hour, 0, 0, 1);
							break;
						case AMBER_ID:
							increment_time_basis(clock_second, clock_minute, clock_hour, 0, 1, 0);
							break;
						case RED_ID:
							clock_second = 0;
							establish_clock_basis(clock_second, clock_minute, clock_hour);
							break;
					}
				} else
					return false;
				}
		}
	}
}
