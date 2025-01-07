#ifndef __TIME_GAME_H
#define __TIME_GAME_H

// Minimum and maximum delay range for the randomly chosen delay before showing the flash
#define MIN_DELAY 2500
#define MAX_DELAY 5000
// Number of time game rounds
#define ROUNDS 3
// Dollars won by beating the curernt time
#define TIME_WIN 1000

extern bool time_game();

#endif
