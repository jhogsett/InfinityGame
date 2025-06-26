#include <Arduino.h>
#include "buffers.h"
#include "buttons.h"
#include "chime.h"
#include "displays.h"
#include "leds.h"
#include "play_data.h"
#include "timeouts.h"
#include "utils.h"
#include "clock.h"
#include "clock_class.h"

// Global clock instance
Clock system_clock;
byte clock_hour, clock_minute, clock_second;

void establish_clock_basis(byte seconds, byte minutes, byte hours) {
	system_clock.setTime(hours, minutes, seconds);
}

long time_in_seconds() {
	return system_clock.getTimeInSeconds();
}

void increment_time_basis(byte &second, byte &minute, byte &hour, byte seconds, byte minutes, byte hours) {
	system_clock.incrementTime(hours, minutes, seconds);
	system_clock.getCurrentTime(hour, minute, second);

	if (!option_clock_24h) {
		if (hour == 0)
			hour = 12;
	}
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

	sprintf_P(display_buffer, PSTR("  %2d %02d %02d  "), effective_hours, minutes, seconds);
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

    char last_chime_hour = -1;

	// clock mode only times out if clock is the idle mode
	while (true) {
		if(settable)
			if((time = millis()) >= idle_timeout)
				return true;

		seconds_to_time(time_in_seconds(), clock_second, clock_minute, clock_hour);

		render_clock_string(clock_second, clock_minute, clock_hour);
		display.show_string(display_buffer);

        if(clock_second == 0 && clock_minute == 0 && clock_hour != last_chime_hour){
            chime(clock_hour);
            last_chime_hour = clock_hour;
        }

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
							system_clock.resetSeconds();
							system_clock.getCurrentTime(clock_hour, clock_minute, clock_second);
							break;
					}
				} else
					return false;
				}
		}
	}
}
