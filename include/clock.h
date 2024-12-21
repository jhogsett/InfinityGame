#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Arduino.h>

#define DEFAULT_TIME_BASIS (1000L * 12L * 60L * 60L)
#define MAX_MS_PER_DAY (1000L * 24L * 60L * 60L)

extern unsigned long clock_basis;
extern unsigned long time_basis;
extern byte clock_hour, clock_minute, clock_second;

extern void establish_clock_basis(byte seconds, byte minutes, byte hours);
extern long time_in_seconds();
extern void increment_time_basis(byte &second, byte &minute, byte &hour, byte seconds, byte minutes, byte hours);
extern void render_clock_string(byte seconds, byte minutes, byte hours);
extern void clock_prompt(byte seconds, byte minutes, byte hours, byte settable=true);

#endif