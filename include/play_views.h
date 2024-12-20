#ifndef __PLAY_VIEW_H
#define __PLAY_VIEW_H

#include <Arduino.h>
#include "buffers.h"

extern void display_purse();
extern void display_win(long win);
extern void display_jackpot(long win);
extern char *numeric_bet_str(long bet);
extern char *standard_bet_str(byte bet);

#endif
