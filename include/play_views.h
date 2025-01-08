#ifndef __PLAY_VIEW_H
#define __PLAY_VIEW_H

#include <Arduino.h>
#include "buffers.h"

// number of times to show player's cash (or effective time of display if no scroll needed)
#define CASH_SHOW_TIMES 1
#define CASH_SHOW_DELAY 750

// number of times to show player's win amount (or effective time of display if no scroll needed)
#define WIN_SHOW_TIMES 2

// number of times to show player's jackpot win (or effective time of display if no scroll needed)
#define JACKPOT_SHOW_TIMES 3

extern void display_purse();
// extern void display_bank();
// extern void display_house();
extern void display_win(long win);
extern void display_jackpot(long win);
extern char *numeric_bet_str(long bet);
extern char *standard_bet_str(byte bet);

#endif
