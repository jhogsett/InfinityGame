#ifndef SLOT_GAME_CLASS_H
#define SLOT_GAME_CLASS_H

#include <Arduino.h>
#include "game_base.h"
#include "word_lists.h" // for NUM_WORDS
#include "HT16K33Disp.h" // For HT16K33Disp

// Constants moved from slot_game.h
#ifndef USE_ALL_WORDS
#define REEL_WORDS 10
#define REEL_BUFFER_LEN (4 + (REEL_WORDS * 6) + 1)
#else
#define REEL_BUFFER_LEN (2 + (NUM_WORDS * 6) + 1)
#endif

class SlotGame : public GameBase {
public:
    SlotGame();
    void setup() override;
    bool loop() override;

private:
    // Helper methods
    void spin_reels();
    void check_for_win();
    bool jackpot_words_chosen(byte word1, byte word2, byte word3);

    // Member variables
    const char **words;
    char text[REEL_BUFFER_LEN];
    bool auto_play;
    long last_bet_amount;
    byte bonus_word;
    byte jackpot_choice1, jackpot_choice2, jackpot_choice3;
    byte choice1, choice2, choice3;
};

#endif // SLOT_GAME_CLASS_H
