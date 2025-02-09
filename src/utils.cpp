#include <Arduino.h>
#include "bank.h"
#include "buffers.h"
#include "play_data.h"
#include "prompts.h"
#include "utils.h"

char * load_f_string(const __FlashStringHelper* f_string, char *override_buffer){
	const char *p = (const char PROGMEM *)f_string;
	return strcpy_P(override_buffer ? override_buffer : fstring_buffer, p);
}

void micros_to_ms(char * buffer, unsigned long micros){
	if(micros == DEFAULT_TIME){
		load_f_string(F("0.0000"), buffer);
	} else {
        unsigned int ms_dec, ms_frac;
        ms_dec = micros / 1000;
        ms_frac = micros % 1000;
        sprintf(buffer, FSTR("%u.%04u ms"), ms_dec, ms_frac);
    }
}

long time_to_seconds(byte second, byte minute, byte hour) {
	long result = (long)second + (60L * (long)minute) + (3600L * (long)hour);
	return result;
}

void seconds_to_time(long seconds, byte &second, byte &minute, byte &hour) {
	hour = seconds / 3600L;
	seconds -= hour * 3600L;
	minute = seconds / 60L;
	seconds -= minute * 60L;
	second = seconds;
}

char *format_long(long num, long basis){
	int negate = (num < 0L) ? -1 : 1;
	if(negate < 0)
		num *= -1L;

	if(basis == 0)
		basis = MONEY_BASIS;

	// this captures and removes the lower three digits in the expanded basis of the number
	// plus removes the basis factor and 1000 for the remaining splits
	long basis_factor = 1000L / basis;
	int units = num % (basis_factor);
	num = (num - units) / (basis_factor);
	units *= basis;

	int thous = ((num % 1000L) - 0) / 1L;
	int mills = ((num % 1000000L) - thous) / 1000L;
	int bills = ((num % 1000000000L) - mills) / 1000000L;

	if(bills > 0)
		sprintf(copy_buffer, FSTR("%d,%03d,%03d,%03d"), bills*negate, mills, thous, units);
	else if(mills > 0)
		sprintf(copy_buffer, FSTR("%d,%03d,%03d"), mills*negate, thous, units);
	else if(thous > 0)
		sprintf(copy_buffer, FSTR("%d,%03d"), thous*negate, units);
	else
		sprintf(copy_buffer, FSTR("%d"), units*negate);

	return copy_buffer;
}

bool title_prompt_int(const char * pattern, int data, bool show_leds, int show_time){
	sprintf(display_buffer, pattern, data);
	return title_prompt(display_buffer, 1, show_leds, show_time == 0 ? ROUND_DELAY : show_time);
}

bool title_prompt_string(const char * pattern, const char * data, bool show_leds, int show_time){
	sprintf(display_buffer, pattern, data);
	return title_prompt(display_buffer, 1, show_leds, show_time == 0 ? ROUND_DELAY : show_time);
}

bool title_prompt_string2(const char * pattern, const char * data1, const char * data2, bool show_leds, int show_time){
	sprintf(display_buffer, pattern, data1, data2);
	return title_prompt(display_buffer, 1, show_leds, show_time == 0 ? ROUND_DELAY : show_time);
}

bool title_prompt_string3(const char * pattern, const char * data1, const char * data2, const char * data3, bool show_leds, int show_time){
	sprintf(display_buffer, pattern, data1, data2, data3);
	return title_prompt(display_buffer, 1, show_leds, show_time == 0 ? ROUND_DELAY : show_time);
}
