#include "bank.h"
#include "betting.h"

#define ROLLING_BET_1 100
#define ROLLING_BET_2 250
#define ROLLING_BET_3 500

long bet_amounts[] = { ROLLING_BET_1 / MONEY_BASIS, ROLLING_BET_2 / MONEY_BASIS, ROLLING_BET_3 / MONEY_BASIS, 0, 0 };
long current_bet = 0;
