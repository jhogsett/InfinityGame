#ifndef __TIME_GAME_H
#define __TIME_GAME_H

// Minimum and maximum delay range for the randomly chosen delay before showing the flash
#define MIN_DELAY 2500
#define MAX_DELAY 5000
// Number of time game rounds
#define TIME_GAME_ROUNDS 3
#define ROUNDS_SHOW_TIME 350
#define BEAT_SHOW_TIME 550

// Dollars won by beating the curernt time
// in MONEY_BASIS units
#define TIME_WIN (1000L / MONEY_BASIS)

#define MODE_FLASH 0
#define MODE_SOUND 1
#define MODE_VIBRATION 2

#define TIMEGAME_MOTOR_ON 12
#define TIMEGAME_MOTOR_OFF 12
#define TIMEGAME_PULSES 6

extern bool time_game();

#endif
