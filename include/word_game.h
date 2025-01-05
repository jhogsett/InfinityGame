#ifndef __WORD_H__
#define __WORD_H__

#define WORD_BUFFER_SIZE 5
#define SCRAMBLE_BUFFER_SIZE 9
#define ADD_CHARS_BUFFER_SIZE 5
#define INSTRUCTIONS_SHOW_TIMES 1
#define CONTROLS_SHOW_TIMES 3
#define CORRECT_WORD_SHOW_TIME 300
#define SUCCESS_SHOW_TIMES 1
#define EXCEEDED_SHOW_TIMES 2
#define MIN_MOVES 1
#define MAX_MOVES 20
#define ADD_CHARS 4
#define SCRAMBLE_SIZE 8
#define SHUFFLE_TIMES_MIN 5
#define SHUFFLE_TIMES_MAX 19

#define BASE_WIN_CASE 5
#define WIN_IN_1_BONUS 10

#define WORD_GAME_PLAY_BET 5

// #define ENABLE_WIN_IN_1

extern char chosen_word[];
extern char scramble_word[];
extern bool new_game;

extern void word_game();

#endif
