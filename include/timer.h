#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>

// current timer value as set by the user
extern byte timer_hour, timer_minute, timer_second;

extern void render_timer_string(byte seconds, byte minutes, byte hours, bool running, bool going_up);
extern void increment_timer(byte &second, byte &minute, byte &hour, byte seconds=1, byte minutes=0, byte hours=0);
extern bool decrement_timer(byte &second, byte &minute, byte &hour, int seconds=1, int minutes=0, int hours=0);
extern bool timer_prompt(byte seconds, byte minutes, byte hours);

#endif
