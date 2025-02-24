#include "prompts.h"
#include "utils.h"
#include "streak.h"

int streak = 0; // -1 means canceled active streak

bool streak_activated(){
    return streak > MIN_STREAK_ACTIVATION;
}

int effective_streak(){
    return streak - MIN_STREAK_ACTIVATION;
}

void cancel_streak(){
    if(streak_activated())
        // canceled already activated streak
        streak = -1;
    else
        // canceled not yet activated streak
        streak = 0;
}

void add_streak(){
    streak++;
}

void reset_streak(){
    streak = 0;
}

unsigned long streak_bonus(){
    unsigned long bonus = 1L;
    if(streak > MIN_STREAK_ACTIVATION){
        bonus = bonus << (long)effective_streak();
    }
    return bonus;
}

void display_bonus(){
    if(streak > MIN_STREAK_ACTIVATION){
        title_prompt_string(FSTR("  BONUS %sX "), format_long(streak_bonus(), 1), true, BONUS_SHOW_DELAY);
    }
}

void display_bonus_gone(){
    if(streak == -1){
        streak = 0;
        title_prompt(FSTR(" BONUS GONE"), BONUS_SHOW_TIMES, false, BONUS_SHOW_DELAY);
    }
}
