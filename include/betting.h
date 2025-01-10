#ifndef __BETTING_H__
#define __BETTING_H__

// Controls the display of types of bets in the Silly SLots game (and others)
#define NUM_BET_AMOUNTS 5
#define BET_ALL 3
#define BET_REPEAT 4

// in MONEY_BASIS_UNITS
#define ROLLING_BET_1 (100L / MONEY_BASIS)
#define ROLLING_BET_2 (250L / MONEY_BASIS)
#define ROLLING_BET_3 (500L / MONEY_BASIS)

extern long bet_amounts[];
extern long current_bet;

#endif
