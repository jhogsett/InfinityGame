#ifndef __CODE_GAME_CLASS_H__
#define __CODE_GAME_CLASS_H__

#include <Arduino.h>
#include "game_base.h"
#include "bank.h" // For MONEY_BASIS

// in MONEY_BASIS units
#define CODE_GAME_WIN_UNIT (100L / MONEY_BASIS)
#define CODE_GAME_PLAY_BET (0L / MONEY_BASIS)

#define MIN_STREAK_ACTIVATION 2

#define CG_WIN_SHOW_DELAY 900
#define LOSE_DELAY 1500

#define BONUS_SHOW_TIMES 1
#define BONUS_SHOW_DELAY 1500

#define CHANCE_OF_NUMBERS 5

class CodeGame : public GameBase {
public:
    CodeGame();
    void setup() override;
    bool loop() override;

private:
    int choose_choices(char *choices, char base, byte range);
    void format_char_display(char c1, char c2, char c3);
    void format_word_display(const char *w1, const char *w2, const char *w3);
    int code_game_round_chars();
    int code_game_round_words(bool rude);
    int code_game_round(bool words, bool rude);

    byte wpm;
    int last_choice;
    int mode;
    bool rude;

    static const int MODE_CHAR = 0;
    static const int MODE_WORD = 1;
};

#endif // __CODE_GAME_CLASS_H__
