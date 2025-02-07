
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
#include "utils.h"
#include "timeouts.h"
#include "code_game.h"

#include "debug.h"

void choose_chars(char *choices, char base, byte range){
    choices[0] = base + random(range);

    choices[1] = base + random(range);
    while(choices[1] == choices[0])
        choices[1] = base + random(range);

    choices[2] = base + random(range);
    while(choices[2] == choices[0] or choices[2] == choices[1])
        choices[2] = base + random(range);
}

// return 1 for win and 0 for loss
// returns -1 in the event of a long press or time out
int code_game_round(){
    load_f_string(F(" Get Ready"), display_buffer);
    title_prompt(display_buffer);
    display.clear();
    delay(ROUND_DELAY);

    randomizer.randomize();
    char choices[3];
    bool letters = random(2) == 1 ? true : false;
    choose_chars(choices, letters ? 'A' : '0', letters ? 26 : 10);
    byte choice = random(3);
    send_morse(choices[choice]);

    delay(ROUND_DELAY);

    sprintf(display_buffer, FSTR("%2c%4c%4c"), choices[0], choices[1], choices[2]);
    bool states[4] = {false, true, true, true};
    int pressed = button_led_prompt(display_buffer, states, false);
    if(pressed == 0 || pressed == -1)
        return -1;

    // button IDs are 1-based
    pressed -= 1;

    int result;
    if(pressed == choice){
        sprintf(display_buffer, FSTR("%2c%4c%4c"), choices[choice], choices[choice], choices[choice]);
        title_prompt(display_buffer, 1, false, ROUND_DELAY);
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
        sprintf(display_buffer, FSTR("%2c%4c%4c"), choices[0], choices[1], choices[2]);

        all_leds.activate_leds(states, true);
        result = 0;
    }
    title_prompt(display_buffer, 1, false, ROUND_DELAY);
    all_leds.deactivate_leds(true);

    return result;
}

bool code_game(){
	title_prompt(FSTR("MorseTrainer"), TITLE_SHOW_TIMES, true);

    // int mode = MODE_FLASH;
	// const char *labels[] = {"LEDS", "BEEP", "BUZZ"};
	// mode = toggle_prompt(FSTR("Choose  %s"), labels, mode, 3, 3);
    // if(mode == -1)
    //     return false;

    // unsigned long show_time = 0;
    // switch(mode){
    //     case MODE_FLASH:
    //         show_time = best_time1;
    //         break;
    //     case MODE_SOUND:
    //         show_time = best_time2;
    //         break;
    //     case MODE_VIBRATION:
    //         show_time = best_time3;
    //         break;
    // }
    // micros_to_ms(copy_buffer, show_time);
	// sprintf(display_buffer, FSTR("Best Score %s ms"), copy_buffer);
	// title_prompt(display_buffer, 1, false, ROUND_DELAY);

    long win = 0;
    int streak = 0; // -1 means canceled

    unsigned long time = millis();
    unsigned long timeout_time = time + option_idle_time;

    int response;
    const bool buttons[] = {false, true, false, true};
    response = button_led_prompt(FSTR("READY   Back"), buttons);
    if(response == 0 || response == -1 || response == RED_ID)
        return false;

    while((time = millis()) < timeout_time){
        display.clear();
        pay_house(use_purse(CODE_GAME_PLAY_BET));
        delay(ROUND_DELAY);

        switch(code_game_round()){
            case -1:
                return false;
            case 0:
                win = 0;
                streak = 0;
                break;
            case 1:
                win = CODE_GAME_WIN_UNIT;
                break;
        }

        if(win > 0){
            // apply the current streak bonus before showing next activation
            if(streak > MIN_STREAK_ACTIVATION){
                unsigned long bonus = 1L << (long)((streak - STREAK_OFFSET) - 1);
                win *= bonus;
            }

            // if(win > 0){
                display_win(win, CG_WIN_SHOW_DELAY);
                streak++;
            // }

            add_to_purse(house_payout(win));
            save_data();
            display_purse(CG_WIN_SHOW_DELAY);

            if(streak > MIN_STREAK_ACTIVATION){
                unsigned long bonus = 1L << (long)((streak - STREAK_OFFSET) - 1);
                sprintf(display_buffer, FSTR("%3sX BONUS"), format_long(bonus, 1));
                title_prompt(display_buffer, BONUS_SHOW_TIMES, true, BONUS_SHOW_DELAY);
            }
        }

        if(streak == -1){
            streak = 0;
            title_prompt(load_f_string(F(" BONUS GONE"), display_buffer), BONUS_SHOW_TIMES, false, BONUS_SHOW_DELAY);
        }

    }

	return false;
}
