#ifndef __PLAY_DATA_H
#define __PLAY_DATA_H

#include <Arduino.h>

// Current save data version
// On start-up if this differs from the EEPROM value, the data is reset to defaults
#define SAVE_DATA_VERSION 2

// Player cash in dollars on resetting defaults
#define DEFAULT_PURSE 1000L

// All money originates in the bank
// All bets are drawn from the bank
#define DEFAULT_BANK 999000L

// All bets are given to the house
// All payouts are from the house
#define DEFAULT_HOUSE 0L

// Display time for interstitial displays during games
#define ROUND_DELAY 750

extern byte save_data_version;

// Whether to play sounds
extern bool option_sound;

// Whether to use vibration feedback (future use)
extern bool option_vibrate;

// Player's Cash
extern long purse;

// Player's best time in The TimeGame
extern unsigned long best_time;

// Whether to display 12 or 24 hour time
extern bool option_clock_24h;

// Whether to show the clock or sleep mode on idle
extern bool option_clock_on_idle;

// Current bank
extern unsigned long bank;

// Current house
extern long house;

// Additional slots for best times (future use)
extern unsigned long best_time1;
extern unsigned long best_time2;
extern unsigned long best_time3;

// Saved data structure version 1
struct SavedData{
	byte version;
	bool option_sound;
	bool option_vibrate;
	unsigned long purse;
	unsigned long best_time;
	bool option_clock_24h;
	bool option_clock_on_idle;
	unsigned long bank;
	unsigned long best_time1;
	unsigned long best_time2;
	unsigned long best_time3;
	long house;
};

extern void load_save_data();
extern void save_data();
extern void reset_options();

#endif
