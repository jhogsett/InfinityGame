#ifndef __SLOT_GAME_H__
#define __SLOT_GAME_H__

// For animating the slot machine wheels, the show time (20ms) before scrolling and the scrolling time (every 20ms)
#define SLOTS_SHOW_TIME   1
#define SLOTS_SCROLL_TIME 19

#define BONUS_SHOW_TIME 1500

// For scoring, having nothing else but the the 'winning word' gets you 1X your bet (returns your bet)
// The 'winning word' is the first word in the Nice and Rude word lists
#define WIN_WORD  1
// Having two matching words gets you 2X your bet
#define WIN_DOUBLE 3
// Having three matching words gets you 5X your bet
#define WIN_TRIPLE 5
// Matching precisely three chosen words, chosen at the time of spinning the reels, gets you 100X your bet
#define WIN_JACKPOT 100
// In the case of double and triple word wins, if the 'winning word' appears also, the winnings are additionally doubled
#define WIN_WORD_BONUS 2
// Default which words are winning words
#define WIN_WORD_CUTOFF 1
// Default best on power-up
#define DEFAULT_BET 10

#ifndef USE_ALL_WORDS
#define REEL_WORDS 10
#define REEL_BUFFER_LEN (4 + (REEL_WORDS * 6) + 1)
#else
#define REEL_BUFFER_LEN (2 + (NUM_WORDS * 6) + 1)
#endif

#define AUTOPLAY_SHOW_TIME 1000
#define AUTOPLAY_BLANK_TIME 500

extern byte choice1, choice2, choice3;

extern bool run_slot_reel(HT16K33Disp * disp, unsigned long time, char * text, const char **words, byte &choice);
extern void slots_round(char * text, const char **words);
extern bool triple_word_chosen();
extern bool double_word_chosen();
extern bool bonus_word_chosen(byte bonus_word);
extern bool jackpot_words_chosen(byte word1, byte word2, byte word3);
extern bool slots_game();

#endif
