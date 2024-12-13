#include <Arduino.h>
#include "buffers.h"
#include "utils.h"

char * load_f_string(const __FlashStringHelper* f_string){
	const char *p = (const char PROGMEM *)f_string;
	return strcpy_P(fstring_buffer, p);
}

void micros_to_ms(char * buffer, unsigned long micros){
	unsigned int ms_dec, ms_frac;
	ms_dec = micros / 1000;
	ms_frac = micros % 1000;
	sprintf(buffer, "%u.%04u", ms_dec, ms_frac);
}
