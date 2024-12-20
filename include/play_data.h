#ifndef __PLAY_DATA_H
#define __PLAY_DATA_H

#include <Arduino.h>

#define SAVE_DATA_VERSION 0
#define DEFAULT_PURSE 1000
#define ROUND_DELAY 750

extern byte save_data_version;
extern bool option_sound;
extern bool option_vibrate;
extern long purse;
extern unsigned long best_time;
extern bool option_clock_24h;
extern bool option_clock_on_idle;

struct SavedData{
	byte version;
	bool option_sound;
	bool option_vibrate;
	unsigned long purse;
	unsigned long best_time;
	bool option_clock_24h;
	bool option_clock_on_idle;
};

extern void load_save_data();
extern void save_data();
extern void reset_options();

#endif
