#ifndef __WORD_H__
#define __WORD_H__

#define WORD_BUFFER_SIZE 5
#define SCRAMBLE_BUFFER_SIZE 9
#define ADD_CHARS_BUFFER_SIZE 5
#define INSTRUCTIONS_SHOW_TIMES 1
#define INSTRUCTIONS_SHOW_DELAY 500
#define CONTROLS_SHOW_TIMES 3
#define CORRECT_WORD_SHOW_TIME 300
#define MOVES_SHOW_TIMES 1
#define MOVES_SHOW_DELAY 400
#define SUCCESS_SHOW_TIMES 1
#define SUCCESS_SHOW_DELAY 300
#define EXCEEDED_SHOW_TIMES 1
#define EXCEEDED_SHOW_DELAY 1000
#define BONUS_SHOW_TIMES 1
#define BONUS_SHOW_DELAY 1500
#define BEAT_SHOW_TIMES 1
#define BEAT_SHOW_DELAY 900
#define WG_WIN_SHOW_DELAY 900

#define MIN_MOVES 1
#define MAX_MOVES 20
#define ADD_CHARS 4
#define SCRAMBLE_SIZE 8
#define SHUFFLE_TIMES_MIN 4
#define SHUFFLE_TIMES_MAX 9

// in MONEY_BASIS units
#define WORD_GAME_WIN_UNIT (100L / MONEY_BASIS)
#define WORD_GAME_PLAY_BET (100L / MONEY_BASIS)

// #define WIN_IN_1_BONUS 10

// subtract this from the streak win count to arrive at the bonus amount
#define STREAK_OFFSET 1

extern char chosen_word[];
extern char scramble_word[];
extern bool new_game;

extern bool word_game();

#endif
