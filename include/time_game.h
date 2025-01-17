#ifndef __TIME_GAME_H
#define __TIME_GAME_H

// Minimum and maximum delay range for the randomly chosen delay before showing the flash
#define MIN_DELAY 2500
#define MAX_DELAY 5000
// Number of time game rounds
#define ROUNDS 3

// Dollars won by beating the curernt time
// in MONEY_BASIS units
#define TIME_WIN (1000L / MONEY_BASIS)

#define MODE_FLASH 0
#define MODE_SOUND 1
#define MODE_VIBRATION 2

#define TIMEGAME_MOTOR_ON 8
#define TIMEGAME_MOTOR_OFF 8
#define TIMEGAME_PULSES 4

extern bool time_game();

#endif
