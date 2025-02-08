
#include <Arduino.h>
#include "bank.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "morse.h"
#include "motor.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "seeding.h"
#include "speaker.h"
#include "streak.h"
#include "utils.h"
#include "timeouts.h"
#include "word_lists.h"
#include "code_game.h"

#include "debug.h"

byte code_game_wpm = 13;
int last_choice = -1;

int choose_choices(char *choices, char base, byte range){
    choices[0] = base + random(range);

    choices[1] = base + random(range);
    while(choices[1] == choices[0])
        choices[1] = base + random(range);

    choices[2] = base + random(range);
    while(choices[2] == choices[0] or choices[2] == choices[1])
        choices[2] = base + random(range);

    byte choice = random(3);
    while(choices[choice] == last_choice)
        choice = random(3);
    last_choice = choices[choice];
    return choice;
}

void format_char_display(char c1, char c2, char c3){
    sprintf(display_buffer, FSTR("%2c%4c%4c"), c1, c2, c3);
}

void format_word_display(const char *w1, const char *w2, const char *w3){
    sprintf(display_buffer, FSTR("%s%s%s"), w1, w2, w3);
}

// return 1 for win and 0 for loss
// returns -1 in the event of a long press or time out
int code_game_round_chars(){
    char choices[3];
    bool letters = random(CHANCE_OF_NUMBERS) != 0 ? true : false;
    byte choice = choose_choices(choices, letters ? 'A' : '0', letters ? 26 : 10);

    send_morse(choices[choice], code_game_wpm);
    delay(ROUND_DELAY);

    format_char_display(choices[0], choices[1], choices[2]);
    bool states[4] = {false, true, true, true};
    int pressed = button_led_prompt(display_buffer, states, false);
    if(pressed == 0 || pressed == -1)
        return -1;
    // button IDs are 1-based
    pressed -= 1;

    int result;
    if(pressed == choice){
        format_char_display(choices[choice], choices[choice], choices[choice]);

        result = 1;
    } else {
        for(int i = 0; i < 3; i++){
            if(i == choice){
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

// return 1 for win and 0 for loss
// returns -1 in the event of a long press or time out
int code_game_round_words(bool rude){
    char choices[3];
    byte choice = choose_choices(choices, 0, 10);

    const char **words = rude ? rude_words : nice_words;

    send_morse(words[(int)choices[(int)choice]], code_game_wpm);

    delay(ROUND_DELAY);

    format_word_display(words[(int)choices[0]], words[(int)choices[1]], words[(int)choices[2]]);
    bool states[4] = {false, true, true, true};
    int pressed = button_led_prompt(display_buffer, states, false);
    if(pressed == 0 || pressed == -1)
        return -1;

    // button IDs are 1-based
    pressed -= 1;

    int result;
    if(pressed == choice){
        format_word_display(words[(int)choices[choice]], words[(int)choices[choice]], words[(int)choices[choice]]);
        result = 1;
    } else {
        sprintf(display_buffer, FSTR("    %s"), words[(int)choices[choice]]);

        result = 0;
    }
    title_prompt(display_buffer, 1, false, ROUND_DELAY);

    return result;
}

int code_game_round(bool words, bool rude){
    title_prompt(FSTR(" Get Ready"));
    display.clear();
    delay(ROUND_DELAY);

    randomizer.randomize();

    if(words){
        return code_game_round_words(rude);
    } else {
        return code_game_round_chars();
    }
}

#define MODE_CHAR 0
#define MODE_WORD 1

bool code_game(){
    last_choice = -1;

    title_prompt(FSTR("MorseTrainer"), TITLE_SHOW_TIMES, true);

    code_game_wpm = prompt_wpm(option_wpm);
    if(code_game_wpm == -1){
        return false;
    }

	const char *labels[] = {"CHAR", "WORD"};
	int mode = toggle_prompt(FSTR("Choose  %s"), labels, MODE_CHAR, 3, 2);
    if(mode == -1)
        return false;

    bool rude = false;
    if(mode == MODE_WORD){
        switch(prompt_nice_or_rude()){
            case -1:
                return false;
            case 1:
                rude = true;
        }
    }

    long win = 0;
    reset_streak();

    unsigned long time = millis();
    unsigned long timeout_time = time + option_idle_time;

    int response;
    const bool buttons[] = {false, true, false, true};
    response = button_led_prompt(FSTR("READY   Back"), buttons);
    if(response == 0 || response == -1 || response == RED_ID)
    return false;

    if(show_instr_long_press())
        return false;

    while((time = millis()) < timeout_time){
        display.clear();
        pay_house(use_purse(CODE_GAME_PLAY_BET));
        delay(ROUND_DELAY);

        switch(code_game_round(mode ? true : false, rude)){
            case -1:
                return false;
            case 0:
                win = 0;
                cancel_streak();
                break;
            case 1:
                win = CODE_GAME_WIN_UNIT;
                break;
        }
        timeout_time = time + option_idle_time;

        if(win > 0){
            win *= streak_bonus();

            display_win(win, CG_WIN_SHOW_DELAY);
            add_streak();

            add_to_purse(house_payout(win));
            save_data();
            display_purse(CG_WIN_SHOW_DELAY);
            display_bonus();
        }

        display_bonus_gone();
    }

	return false;
}
