#include "time_game_class.h"
#include "Arduino.h"
#include "bank.h"
#include "buffers.h"
#include "buttons.h"
#include "displays.h"
#include "leds.h"
#include "motor.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "speaker.h"
#include "string_constants.h"
#include "utils.h"
#include "time_game.h" // for old constants
#include "timeouts.h"
#include "debug.h"

TimeGame::TimeGame() : GameBase(F("The TimeGame"), 0) {
    current_round = 0;
}

void TimeGame::setup() {
    const char *labels[] = {"LEDS", "BEEP", "BUZZ"};
	mode = toggle_prompt(FSTR("Choose  %s"), labels, 0, 3, 3);
    if(mode == -1) {
        set_finished(true);
        return;
    }

    best_time = render_best_time_per_mode(copy_buffer, mode);
    if(best_time != DEFAULT_TIME)
        title_prompt_string(FSTR("Best Score %s"), copy_buffer, false, ROUND_DELAY);

    title_prompt_int(FSTR("%3d Rounds"), TIME_GAME_ROUNDS, false, ROUNDS_SHOW_TIME);
}

bool TimeGame::loop() {
    if (current_round >= TIME_GAME_ROUNDS) {
        // Game finished, calculate final score and exit
        mean_time /= TIME_GAME_ROUNDS;
        micros_to_ms(copy_buffer, mean_time);
        title_prompt_string(FSTR("SCORE %s"), copy_buffer, false, ROUND_DELAY);

        bool new_best_per_mode = false;
        char label[5];
        switch(mode){
            case MODE_FLASH:
                if(mean_time < best_time1){
                    best_time1 = mean_time;
                    new_best_per_mode = true;
                    strcpy_P(label, STR_LEDS);
                }
                break;
            case MODE_SOUND:
                if(mean_time < best_time2){
                    best_time2 = mean_time;
                    new_best_per_mode = true;
                    strcpy_P(label, STR_BEEP);
                }
                break;
            case MODE_VIBRATION:
                if(mean_time < best_time3){
                    best_time3 = mean_time;
                    new_best_per_mode = true;
                    strcpy_P(label, STR_BUZZ);
                }
                break;
        }
        if(new_best_per_mode){
            micros_to_ms(copy_buffer, mean_time);
            title_prompt_string2(FSTR("*NEW BEST %s %s"), label, copy_buffer, true, ROUND_DELAY);
        }

        if(mean_time < best_time){
            best_time = mean_time;
            win = TIME_WIN;
            micros_to_ms(copy_buffer, mean_time);
            title_prompt_string(FSTR("*NEW OVERALL BEST %s"), copy_buffer, true, ROUND_DELAY);
            pay_player(win);
        }

        set_finished(true); // Signal that the game is over
        return false; // Exit loop
    }

    // -- Per-round logic starts here --

    if (current_round == 0) {
        best_time = render_best_time_per_mode(copy_buffer, mode);
        if(best_time != DEFAULT_TIME)
            title_prompt_string(FSTR("Beat %s"), copy_buffer, false, BEAT_SHOW_TIME);
    }

    display.clear();
    delay(ROUND_DELAY);

    bool fault_protect = true;
    unsigned long reaction_time = 0;
    while(fault_protect){
        button_leds.activate_all(true);

        switch(mode){
            case MODE_FLASH:
                display.scroll_string(FSTR("Wait 4 FLASH"), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
                break;
            case MODE_SOUND:
                display.scroll_string(FSTR("Wait 4 BEEP"), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
                break;
            case MODE_VIBRATION:
                display.scroll_string(FSTR("Wait 4 BUZZ"), DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
                break;
        }
        display.clear();
        button_leds.activate_all(false);

        int del = random(MIN_DELAY, MAX_DELAY+1);

        unsigned long delay_timeout = millis() + del;

        bool fault = false;
        while(millis() < delay_timeout)
            if(digitalRead(ANY_BUTTON) == HIGH){
                fault = true;
                break;
            }
        if(fault)
            continue;
        fault_protect = false;

        switch(mode){
            case MODE_FLASH:
                panel_leds.begin_flash(false, 0);
                break;
        }

        unsigned long start_time = micros();

        switch(mode){
            case MODE_FLASH:
                panel_leds.step_flash(millis());
                break;
            case MODE_SOUND:
                beep();
                break;
            case MODE_VIBRATION:
                vibrate_raw(TIMEGAME_PULSES, TIMEGAME_MOTOR_ON, TIMEGAME_MOTOR_OFF);
                break;
        }

        while(digitalRead(ANY_BUTTON) == LOW){
            switch(mode){
                case MODE_FLASH:
                    panel_leds.step_flash(millis());
                    break;
            }
        }

        reaction_time = micros() - start_time;

        while(digitalRead(ANY_BUTTON) == HIGH){
            switch(mode){
                case MODE_FLASH:
                    panel_leds.step_flash(millis());
                    break;
            }
        }
    }

    mean_time += reaction_time;

    micros_to_ms(copy_buffer, reaction_time);
    title_prompt_string(FSTR(" %s"), copy_buffer, false, ROUND_DELAY);
    delay(ROUND_DELAY);
    display.clear();

    current_round++;
    return true; // Continue to the next round
}

unsigned long TimeGame::render_best_time_per_mode(char * buffer, int mode){
    unsigned long show_time = 0;
    switch(mode){
        case MODE_FLASH:
            show_time = best_time1;
            break;
        case MODE_SOUND:
            show_time = best_time2;
            break;
        case MODE_VIBRATION:
            show_time = best_time3;
            break;
    }
    micros_to_ms(copy_buffer, show_time);
    return show_time;
}
