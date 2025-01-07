#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Arduino.h>

// The default clock time 12:00:00
#define DEFAULT_TIME_BASIS (1000L * 12L * 60L * 60L)

// The milliseonds in a day
// This is used to advance the clock ahead from 23:59:59 to 00:00:00 on a daily basis
#define MAX_MS_PER_DAY (1000L * 24L * 60L * 60L)

// "Time Basis" is the offset in millseconds from the "Clock Basis" (described below)
extern unsigned long time_basis;

// When the user syncs the clock to zero-seconds, a new 24-hour time window is established, called the "Clock Basis."
// As the millis() values increases on its own over time (automatically via the Arduino system), the Clock Basis is
//   subtracted from it to compute the elapsed time since the start of the time window.
// At the same time, the 'hours' and 'minutes' (and '0' seconds) are saved in milliseconds as the new "Time Basis."
// This establishes what should be displayed as the curernt time.
//
// To compute the current time for display:
// 1) current milli() values - Clock Basis = Elapsed Time since zero-second syncing & setting the time
// 2) computed Elapsed Time in milliseconds + an ealier set time in milliseonds = the current time
//
// As long as the device is kept running, the time window is automatically rolled over to a new 24 hour window each day
extern unsigned long clock_basis;

// used while displaying and setting the time
extern byte clock_hour, clock_minute, clock_second;

extern void establish_clock_basis(byte seconds, byte minutes, byte hours);
extern long time_in_seconds();
extern void increment_time_basis(byte &second, byte &minute, byte &hour, byte seconds, byte minutes, byte hours);
extern void render_clock_string(byte seconds, byte minutes, byte hours);
extern bool clock_prompt(byte seconds, byte minutes, byte hours, byte settable=true);

#endif