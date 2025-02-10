#ifndef __STREAK_H__
#define __STREAK_H__

// activate streak bonus after the third win in a row
#define MIN_STREAK_ACTIVATION 2

#define BONUS_SHOW_DELAY 1500
#define BONUS_SHOW_TIMES 1

extern int streak;

extern bool streak_activated();
extern int effective_streak();
extern void cancel_streak();
extern void add_streak();
extern void reset_streak();
unsigned long streak_bonus();
extern void display_bonus();
extern void display_bonus_gone();

#endif
