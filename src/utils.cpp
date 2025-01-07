#include <Arduino.h>
#include "buffers.h"
#include "utils.h"

char * load_f_string(const __FlashStringHelper* f_string, char *override_buffer){
	const char *p = (const char PROGMEM *)f_string;
	return strcpy_P(override_buffer ? override_buffer : fstring_buffer, p);
}

void micros_to_ms(char * buffer, unsigned long micros){
	unsigned int ms_dec, ms_frac;
	ms_dec = micros / 1000;
	ms_frac = micros % 1000;
	sprintf(buffer, FSTR("%u.%04u"), ms_dec, ms_frac);
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
