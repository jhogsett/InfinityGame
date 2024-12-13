#ifndef __SLOT_GAME_H__
#define __SLOT_GAME_H__

#define SLOTS_SHOW_TIME   20
#define SLOTS_SCROLL_TIME 20

#define WIN_TRIPLE 5
#define WIN_DOUBLE 2
#define WIN_WORD  1
#define WIN_WORD_CUTOFF 1
#define DEFAULT_BET 10
#define WIN_WORD_BONUS 2
#define WIN_JACKPOT 100

#define NUM_WORDS 10

extern const char *rude_words[];
extern const char *nice_words[];

extern byte choice1, choice2, choice3;

extern void slots_round(bool rude);
extern bool triple_word_chosen();
extern bool double_word_chosen();
extern bool special_word_chosen();
extern bool jackpot_words_chosen(byte word1, byte word2, byte word3);
extern void slots_game();

#endif
