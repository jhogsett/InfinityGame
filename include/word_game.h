#ifndef __WORD_H__
#define __WORD_H__

#define WORD_BUFFER_SIZE 5
#define SCRAMBLE_BUFFER_SIZE 9
#define ADD_CHARS_BUFFER_SIZE 5
#define INSTRUCTION_SHOW_TIMES 1
#define SUCCESS_SHOW_TIMES 1
#define EXCEEDED_SHOW_TIMES 2
#define MIN_MOVES 1
#define MAX_MOVES 20
#define ADD_CHARS 4
#define SCRAMBLE_SIZE 8

#define BASE_WIN_CASE 5
#define WIN_IN_1_BONUS 10

// #define ENABLE_WIN_IN_1

extern char chosen_word[];
extern char scramble_word[];

extern void word_game();

#endif
