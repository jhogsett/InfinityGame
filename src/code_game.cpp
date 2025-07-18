#include <Arduino.h>
#include "bank.h"
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
#include "code_game_class.h"

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
    sprintf_P(display_buffer, PSTR("%2c%4c%4c"), c1, c2, c3);
}

void format_word_display(const char *w1, const char *w2, const char *w3){
    sprintf_P(display_buffer, PSTR("%s%s%s"), w1, w2, w3);
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
    byte choice = choose_choices(choices, 0, NUM_WORDS);

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
        sprintf_P(display_buffer, PSTR("    %s"), words[(int)choices[choice]]);

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

bool code_game() {
    CodeGame game;
    return game.run();
}
