#include <Arduino.h>
#include <EEPROM.h>
#include "play_data.h"

bool option_sound = false;
bool option_vibrate = false;
long purse = DEFAULT_PURSE;
unsigned long best_time = (unsigned long)-1;
bool option_clock_24h = false;
bool option_clock_on_idle = false;

void load_save_data(){
  SavedData saved_data;
  EEPROM.get(0, saved_data);
  option_sound = saved_data.option_sound;
  option_vibrate = saved_data.option_vibrate;
  purse = saved_data.purse;
  best_time = saved_data.best_time;
  option_clock_24h = saved_data.option_clock_24h;
  option_clock_on_idle = saved_data.option_clock_on_idle;
}

void save_data(){
  SavedData saved_data;
  saved_data.option_sound = option_sound;
  saved_data.option_vibrate = option_vibrate;
  saved_data.purse = purse;
  saved_data.best_time = best_time;
  saved_data.option_clock_24h = option_clock_24h;
  saved_data.option_clock_on_idle = option_clock_on_idle;
  EEPROM.put(0, saved_data);
}

void reset_options(){
  option_sound = true;
  option_vibrate = true;
  purse = DEFAULT_PURSE;
  best_time = (unsigned long)-1;
  option_clock_24h = false;
  option_clock_on_idle = false;
  save_data();
}
