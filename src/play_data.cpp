#include <Arduino.h>
#include <EEPROM.h>
#include "bank.h"
#include "play_data.h"

// when adding new persisted play data, search for ##DATA

bool option_sound = false;
bool option_vibrate = false;
long purse = DEFAULT_PURSE;
unsigned long best_time = (unsigned long)-1;
bool option_clock_24h = false;
bool option_clock_on_idle = false;
unsigned long bank = DEFAULT_BANK;
unsigned long best_time1 = (unsigned long)-1;
unsigned long best_time2 = (unsigned long)-1;
unsigned long best_time3 = (unsigned long)-1;
long house = DEFAULT_HOUSE;
long gang = DEFAULT_GANG;
unsigned long option_idle_time = DEFAULT_IDLE_TIME;

// ##DATA Add new persisted play data veriables here


void load_save_data(){
	SavedData saved_data;
	EEPROM.get(0, saved_data);

	if(saved_data.version != SAVE_DATA_VERSION){
		reset_options();
		return;
	}

	option_sound = saved_data.option_sound;
	option_vibrate = saved_data.option_vibrate;
	purse = saved_data.purse;
	best_time = saved_data.best_time;
	option_clock_24h = saved_data.option_clock_24h;
	option_clock_on_idle = saved_data.option_clock_on_idle;
	bank = saved_data.bank;
	best_time1 = saved_data.best_time1;
	best_time2 = saved_data.best_time2;
	best_time3 = saved_data.best_time3;
	house = saved_data.house;
	gang = saved_data.gang;
	option_idle_time = saved_data.option_idle_time;

	// ##DATA Load new persisted play data variables into memory here
}

void save_data(){
	SavedData saved_data;
	saved_data.version = SAVE_DATA_VERSION;
	saved_data.option_sound = option_sound;
	saved_data.option_vibrate = option_vibrate;
	saved_data.purse = purse;
	saved_data.best_time = best_time;
	saved_data.option_clock_24h = option_clock_24h;
	saved_data.option_clock_on_idle = option_clock_on_idle;
	saved_data.bank = bank;
	saved_data.best_time1 = best_time1;
	saved_data.best_time2 = best_time2;
	saved_data.best_time3 = best_time3;
	saved_data.house = house;
	saved_data.gang = gang;
	saved_data.option_idle_time = option_idle_time;

	// ##DATA Store new persisted play data veriables in the persistent structure here

	EEPROM.put(0, saved_data);
}

typedef void (*VoidFunc)(void);

void reset_device(){
	VoidFunc p = NULL;
	p();
}

bool reset_options(){
	option_sound = true;
	option_vibrate = true;
	purse = DEFAULT_PURSE;
	best_time = DEFAULT_TIME;
	option_clock_24h = false;
	option_clock_on_idle = true;
	bank = DEFAULT_BANK;
	best_time1 = DEFAULT_TIME;
	best_time2 = DEFAULT_TIME;
	best_time3 = DEFAULT_TIME;
	house = DEFAULT_HOUSE;
	gang = DEFAULT_GANG;
	option_idle_time = DEFAULT_IDLE_TIME;

	// ##DATA Reset new persisted play data veriables to default variables here

	save_data();

	reset_device();
	return false;
}
