#ifndef __PLAY_VIEW_H
#define __PLAY_VIEW_H

#include <Arduino.h>
#include "buffers.h"

// number of times to show player's cash (or effective time of display if no scroll needed)
#define CASH_SHOW_TIMES 1
#define CASH_SHOW_DELAY 750

#define BALANCES_SHOW_TIMES 1
#define BALANCES_SHOW_DELAY 750

// number of times to show player's win amount (or effective time of display if no scroll needed)
#define WIN_SHOW_TIMES 1
#define WIN_SHOW_DELAY 750

// number of times to show player's jackpot win (or effective time of display if no scroll needed)
#define JACKPOT_SHOW_TIMES 2

extern bool display_scores();
extern bool display_balances();
extern void display_purse(int delay=0);
// extern void display_bank();
// extern void display_house();
extern void display_win(unsigned long win, int delay=0);
extern void display_jackpot(unsigned long win);
extern char *format_long(long num, long basis=0);
// extern char *numeric_bet_str(long bet);
extern char *standard_bet_str(byte bet);

#endif
