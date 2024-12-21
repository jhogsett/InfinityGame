#include <Arduino.h>
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "play_data.h"
#include "utils.h"
#include "clock.h"



// #define DEFAULT_TIME_BASIS (1000L * 12L * 3600L)

unsigned long clock_basis = 0L;
unsigned long time_basis = DEFAULT_TIME_BASIS;
byte clock_hour, clock_minute, clock_second;

void establish_clock_basis(byte seconds, byte minutes, byte hours) {
	clock_basis = millis();
	long total_seconds = time_to_seconds(seconds, minutes, hours);
	time_basis = total_seconds * 1000L;
}

// #define MAX_MS_PER_DAY (1000L * 24L * 60L * 60L)

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

	if (effective_hours < 10)
		sprintf(display_buffer, load_f_string(F("   %1d %02d %02d  ")), effective_hours, minutes, seconds);
	else
		sprintf(display_buffer, load_f_string(F("  %2d %02d %02d  ")), effective_hours, minutes, seconds);
}

void clock_prompt(byte seconds, byte minutes, byte hours, byte settable) {
	clock_hour = hours;
	clock_minute = minutes;
	clock_second = seconds;

	render_clock_string(clock_second, clock_minute, clock_hour);
	display.show_string(display_buffer);
	// clock mode never times out
	while (true) {
		seconds_to_time(time_in_seconds(), clock_second, clock_minute, clock_hour);

		render_clock_string(clock_second, clock_minute, clock_hour);
		display.show_string(display_buffer);

		if (button_pressed()) {
			all_leds.activate_leds(button_states, true);
			int long_press_state;
			while ((long_press_state = wait_on_long_press()) == 0)
				;

			all_leds.deactivate_leds(true);
			if (long_press_state == 1) {
				return;
			} else {
				if (settable) {
					if (button_states[GREEN_ID]) {
						increment_time_basis(clock_second, clock_minute, clock_hour, 0, 0, 1);
					} else if (button_states[AMBER_ID])
						increment_time_basis(clock_second, clock_minute, clock_hour, 0, 1, 0);
					else if (button_states[RED_ID]) {
						clock_second = 0;
						establish_clock_basis(clock_second, clock_minute, clock_hour);
					}
			} else
				return;
			}
		}
	}
}
