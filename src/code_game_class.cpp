#include "code_game_class.h"
#include "buffers.h"
#include "displays.h"
#include "leds.h"
#include "morse.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "seeding.h"
#include "streak.h"
#include "utils.h"
#include "word_lists.h"
#include "options_mode.h" // for option_wpm, option_idle_time
#include "bank.h"

CodeGame::CodeGame() : GameBase(F("MorseTrainer"), CODE_GAME_PLAY_BET) {
    last_choice = -1;
    wpm = 13;
    mode = MODE_CHAR;
    rude = false;
}

void CodeGame::setup() {
    last_choice = -1;

    wpm = prompt_wpm(option_wpm);
    if (wpm == 0) { // Assuming 0 is an invalid WPM, indicating cancellation
        set_finished(true);
        return;
    }

    const char *labels[] = {"CHAR", "WORD"};
    mode = toggle_prompt(FSTR("Choose  %s"), labels, MODE_CHAR, 3, 2);
    if (mode == -1) {
        set_finished(true);
        return;
    }

    if (mode == MODE_WORD) {
        switch (prompt_nice_or_rude()) {
            case -1:
                set_finished(true);
                return;
            case 1:
                rude = true;
        }
    }

    if (!prompt_ready()) {
        set_finished(true);
        return;
    }

    if (show_instr_long_press()) {
        set_finished(true);
        return;
    }
}

bool CodeGame::loop() {
    reset_timeout();

    display.clear();
    pay_house(use_purse(play_bet));
    delay(ROUND_DELAY);

    switch (code_game_round(mode == MODE_WORD, rude)) {
        case -1:
            set_finished(true);
            return false;
        case 0:
            win = 0;
            cancel_streak();
            break;
        case 1:
            win = CODE_GAME_WIN_UNIT;
            break;
    }

    if (win > 0) {
        win *= streak_bonus();
        display_win(win, CG_WIN_SHOW_DELAY);
        add_streak();
        pay_player(win);
    } else {
        title_prompt(FSTR("    No Win"), 1, false, LOSE_DELAY);
    }

    return true; // Continue game
}

int CodeGame::choose_choices(char *choices, char base, byte range) {
    choices[0] = base + random(range);

    choices[1] = base + random(range);
    while (choices[1] == choices[0])
        choices[1] = base + random(range);

    choices[2] = base + random(range);
    while (choices[2] == choices[0] || choices[2] == choices[1])
        choices[2] = base + random(range);

    byte choice = random(3);
    while (choices[choice] == last_choice)
        choice = random(3);
    last_choice = choices[choice];
    return choice;
}

void CodeGame::format_char_display(char c1, char c2, char c3) {
    sprintf_P(display_buffer, PSTR("%2c%4c%4c"), c1, c2, c3);
}

void CodeGame::format_word_display(const char *w1, const char *w2, const char *w3) {
    sprintf_P(display_buffer, PSTR("%s%s%s"), w1, w2, w3);
}

int CodeGame::code_game_round_chars() {
    char choices[3];
    bool letters = random(CHANCE_OF_NUMBERS) != 0;
    byte choice = choose_choices(choices, letters ? 'A' : '0', letters ? 26 : 10);

    send_morse(choices[choice], wpm);
    delay(ROUND_DELAY);

    format_char_display(choices[0], choices[1], choices[2]);
    bool states[4] = {false, true, true, true};
    int pressed = button_led_prompt(display_buffer, states, false);
    if (pressed == 0 || pressed == -1)
        return -1;
    pressed -= 1; // button IDs are 1-based

    int result;
    if (pressed == choice) {
        format_char_display(choices[choice], choices[choice], choices[choice]);
        result = 1;
    } else {
        for (int i = 0; i < 3; i++) {
            if (i == choice) {
                states[i + 1] = true;
            } else {
                choices[i] = ' ';
                states[i + 1] = false;
            }
        }
        format_char_display(choices[0], choices[1], choices[2]);
        all_leds.activate_leds(states, true);
        result = 0;
    }
    title_prompt(display_buffer, 1, false, ROUND_DELAY);
    all_leds.deactivate_leds(true);

    return result;
}

int CodeGame::code_game_round_words(bool rude) {
    char choices[3];
    byte choice = choose_choices(choices, 0, NUM_WORDS);

    const char **words = rude ? rude_words : nice_words;

    send_morse(words[(int)choices[(int)choice]], wpm);
    delay(ROUND_DELAY);

    format_word_display(words[(int)choices[0]], words[(int)choices[1]], words[(int)choices[2]]);
    bool states[4] = {false, true, true, true};
    int pressed = button_led_prompt(display_buffer, states, false);
    if (pressed == 0 || pressed == -1)
        return -1;

    pressed -= 1; // button IDs are 1-based

    int result;
    if (pressed == choice) {
        format_word_display(words[(int)choices[choice]], words[(int)choices[choice]], words[(int)choices[choice]]);
        result = 1;
    } else {
        sprintf_P(display_buffer, PSTR("    %s"), words[(int)choices[choice]]);
        result = 0;
    }
    title_prompt(display_buffer, 1, false, ROUND_DELAY);

    return result;
}

int CodeGame::code_game_round(bool words, bool rude) {
    title_prompt(FSTR(" Get Ready"));
    display.clear();
    delay(ROUND_DELAY);

    randomizer.randomize();

    if (words) {
        return code_game_round_words(rude);
    } else {
        return code_game_round_chars();
    }
}
