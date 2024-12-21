#ifndef __UTILS_H__
#define __UTILS_H__

extern char * load_f_string(const __FlashStringHelper* f_string);
extern void micros_to_ms(char * buffer, unsigned long micros);
extern long time_to_seconds(byte second, byte minute, byte hour);
extern void seconds_to_time(long seconds, byte &second, byte &minute, byte &hour);

#endif