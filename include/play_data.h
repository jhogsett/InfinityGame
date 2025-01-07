#ifndef __PLAY_DATA_H
#define __PLAY_DATA_H

#include <Arduino.h>

// when adding new persisted play data, search for ##DATA

// ##DATA Increment the save data version to force upgraded devices to auto-reset after programming
// Current save data version
// On start-up if this differs from the EEPROM value, the data is reset to defaults
#define SAVE_DATA_VERSION 2

// All bets are paid from the player purse
// all wins received are kept in the player purse
// the player purse draws money from the syndiate when needed
#define DEFAULT_PURSE 0L

// All money originates in the bank
#define DEFAULT_BANK 1000000000L

// All bets are paid to the house
// All payouts are from the house
// The house draws money from the bank when needed
#define DEFAULT_HOUSE 0L

// The gang robs money from the bank
// Player cash comes from the gang
#define DEFAULT_GANG 0L

// the longest possible count of milliseconds
#define DEFAULT_TIME ((unsigned long)-1)

// default milliseconds until device goes into idle mode
#define DEFAULT_IDLE_TIME (5 * 60 * 1000)

// ##DATA add new defaults on play data reset here


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

// Additional slots for best times (future use)
extern unsigned long best_time1;
extern unsigned long best_time2;
extern unsigned long best_time3;

// Current house
extern long house;

// Current gang
extern long gang;

// Idle Time in milliseconds
extern unsigned long idle_time;

// ##DATA Add 'extern's for new persisted play data veriables here


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
	long gang;
	unsigned long idle_time;

	// ##DATA Add new persisted data types here
};

extern void load_save_data();
extern void save_data();
extern bool reset_options();

#endif
