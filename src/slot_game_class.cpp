#include "slot_game_class.h"
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "timeouts.h"
#include "utils.h"
#include "seeding.h"
#include "slot_game.h"
#include "word_lists.h"

SlotGame::SlotGame() : GameBase(F("Silly Slots"), 0) {}

void SlotGame::setup() {
    title_prompt(FSTR("Silly Slots"), TITLE_SHOW_TIMES, true);

    randomizer.randomize();
    bonus_word = random(NUM_WORDS);
    randomizer.randomize();
    jackpot_choice1 = random(NUM_WORDS);
    randomizer.randomize();
    jackpot_choice2 = random(NUM_WORDS);
    randomizer.randomize();
    jackpot_choice3 = random(NUM_WORDS);

    bool rude = false;
    switch (prompt_nice_or_rude()) {
    case -1:
        return;
    case 1:
        rude = true;
    }

    words = rude ? rude_words : nice_words;

#ifndef USE_ALL_WORDS
    int reel_words[REEL_WORDS];
    random_unique(REEL_WORDS, NUM_WORDS, reel_words);

    strcpy_P(this->text, PSTR("    "));
    for (int i = 0; i < REEL_WORDS; i++) {
        strcat(this->text, words[reel_words[i]]);
        if (i < REEL_WORDS - 1) {
            strcat_P(this->text, PSTR("  "));
        }
    }
#else
    this->text[0] = '\0';
    for (int i = 0; i < NUM_WORDS; i++) {
        strcat(this->text, words[i]);
        if (i < NUM_WORDS - 1) {
            strcat_P(this->text, PSTR(" "));
        }
    }
#endif

    title_prompt(FSTR(" Bonus Word"), 1, false, ROUND_DELAY);
    sprintf_P(display_buffer, PSTR("%s%s%s"), words[bonus_word], words[bonus_word], words[bonus_word]);
    title_prompt(display_buffer, 1, true, BONUS_SHOW_TIME);
    display.clear();
    delay(ROUND_DELAY);
    auto_play = false;
    last_bet_amount = 0L;
    win = 0L;
}

bool SlotGame::loop() {
    bet_amounts[BET_ALL] = get_purse();
    bet_amounts[BET_REPEAT] = last_bet_amount;

    if (auto_play) {
        display.clear();
        delay(AUTOPLAY_BLANK_TIME);

        if (bet_amounts[BET_REPEAT] > 0L) {
            if (bet_amounts[BET_ALL] <= 0) {
                current_bet = BET_REPEAT;
            } else if (win > 0L) {
                if (win <= bet_amounts[BET_REPEAT]) {
                    current_bet = BET_ALL;
                } else {
                    current_bet = BET_ALL;
                }
            } else {
                if (bet_amounts[BET_REPEAT] > bet_amounts[BET_ALL])
                    current_bet = BET_REPEAT;
                else
                    current_bet = BET_ALL;
            }
        } else {
            current_bet = BET_ALL;
        }

        sprintf_P(display_buffer, PSTR("AUTO BET $%s"), format_long(bet_amounts[current_bet]));
        if (title_prompt(display_buffer, 1, true, AUTOPLAY_SHOW_TIME, LEDHandler::STYLE_RANDOM, AUTOPLAY_LEDS_TIME)) {
            return false;
        }
    } else {
        sprintf_P(display_buffer, PSTR("BET %s Back"), standard_bet_str(current_bet));
        const bool states[] = { false, true, false, false };
        int response = button_led_prompt(display_buffer, states, true);
        switch (response) {
        case -1: // timeout
            return false;
        case 0: // long press
            if (validated_button_states[GREEN_ID]) {
                current_bet = BET_REPEAT;
            } else if (validated_button_states[AMBER_ID]) {
                current_bet = BET_ALL;
            } else if (validated_button_states[RED_ID]) {
                if (auto_play_enabled) {
                    auto_play = true;
                    current_bet = BET_REPEAT;
                } else {
                    return false;
                }
            }
            break;
        case 1:
            break;
        case 2:
            current_bet++;
            if (current_bet >= NUM_BET_AMOUNTS)
                current_bet = 0;

            sprintf_P(display_buffer, PSTR("    %s"), standard_bet_str(current_bet));
            disp2.scroll_string(display_buffer, 1, OPTION_FLIP_SCROLL_TIME);
            return loop(); // restart to show new bet
        case 3:
            return false;
        }
    }

    bool jackpot = false;

    last_bet_amount = bet_amounts[current_bet];
    pay_house(use_purse(last_bet_amount));
    save_data();

    spin_reels();

    while (button_pressed());

    strcpy(display_buffer, words[choice1]);
    strcat(display_buffer, words[choice2]);
    strcat(display_buffer, words[choice3]);
    title_prompt(display_buffer, 1, true);

    check_for_win();

    if (win > 0) {
        if (jackpot_words_chosen(jackpot_choice1, jackpot_choice2, jackpot_choice3)) {
            display_jackpot(win);
        } else {
            display_win(win);
        }
        pay_player(win);
        save_data();
        return true; // win
    } else {
        delay(ROUND_DELAY);
        title_prompt(FSTR("   No Win"), 1, false, 1000);
        return true; // loss
    }
}

void SlotGame::spin_reels() {
    disp1.begin_scroll_loop(1);
    disp2.begin_scroll_loop(2);
    disp3.begin_scroll_loop(3);

    bool running1 = true;
    bool running2 = true;
    bool running3 = true;
    while (running1 || running2 || running3) {
        unsigned long time = millis();

        if (running1) {
            running1 = disp1.loop_scroll_string(time, this->text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
            if (!running1) {
                randomizer.randomize();
                choice1 = random(NUM_WORDS);
                disp1.show_string(words[choice1]);
            }
        }

        if (running2) {
            running2 = disp2.loop_scroll_string(time, this->text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
            if (!running2) {
                randomizer.randomize();
                choice2 = random(NUM_WORDS);
                disp2.show_string(words[choice2]);
            }
        }

        if (running3) {
            running3 = disp3.loop_scroll_string(time, this->text, SLOTS_SHOW_TIME, SLOTS_SCROLL_TIME);
            if (!running3) {
                randomizer.randomize();
                choice3 = random(NUM_WORDS);
                disp3.show_string(words[choice3]);
            }
        }
    }
}

void SlotGame::check_for_win() {
    long win_factor = 0;
    bool word_bonus = (choice1 == bonus_word) || (choice2 == bonus_word) || (choice3 == bonus_word);

    if (jackpot_words_chosen(jackpot_choice1, jackpot_choice2, jackpot_choice3)) {
        win_factor = WIN_JACKPOT;
    } else if ((choice1 == choice2) && (choice2 == choice3)) {
        win_factor = WIN_TRIPLE;
        if (word_bonus)
            win_factor *= WIN_WORD_BONUS;
    } else if ((choice1 == choice2) || (choice2 == choice3) || (choice1 == choice3)) {
        win_factor = WIN_DOUBLE;
        if (word_bonus)
            win_factor *= WIN_WORD_BONUS;
    } else if (word_bonus) {
        win_factor = WIN_WORD;
    }

    win = bet_amounts[current_bet] * win_factor;
}

bool SlotGame::jackpot_words_chosen(byte word1, byte word2, byte word3) {
    return (choice1 == word1) && (choice2 == word2) && (choice3 == word3);
}
