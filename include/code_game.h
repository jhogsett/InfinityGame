#ifndef __CODE_GAME_H__
#define __CODE_GAME_H__


// Minimum and maximum delay range for the randomly chosen delay before showing the flash
#define MIN_DELAY 2500
#define MAX_DELAY 5000
// Number of time game rounds
#define ROUNDS 3

// in MONEY_BASIS units
#define CODE_GAME_WIN_UNIT (100L / MONEY_BASIS)
#define CODE_GAME_PLAY_BET (100L / MONEY_BASIS)

// activate streak bonus after two beats in a row
#define MIN_STREAK_ACTIVATION 2

// subtract this from the streak win count to arrive at the bonus amount
#define STREAK_OFFSET 1

#define CG_WIN_SHOW_DELAY 900

#define BONUS_SHOW_TIMES 1
#define BONUS_SHOW_DELAY 1500

#define CHANCE_OF_NUMBERS 10

extern bool code_game();

#endif
