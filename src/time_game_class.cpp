#include "time_game_class.h"
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
#include "timeouts.h"
#include "debug.h"

TimeGame::TimeGame() : GameBase(F("The TimeGame"), 0) {}

unsigned long TimeGame::renderBestTimePerMode(int mode){
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

void TimeGame::setup() {
    if(title_prompt(FSTR("The TimeGame"), TITLE_SHOW_TIMES, true)) {
        setFinished(true);
        return;
    }
    
    const char *labels[] = {"LEDS", "BEEP", "BUZZ"};
    mode = toggle_prompt(FSTR("Choose  %s"), labels, MODE_FLASH, 3, 3);
    if(mode == -1) {
        setFinished(true);
        return;
    }

    unsigned long best_time = renderBestTimePerMode(mode);
    if(best_time != DEFAULT_TIME)
        title_prompt_string(FSTR("Best Score %s"), copy_buffer, false, ROUND_DELAY);

    round = 0;
    mean_time = 0;
}

bool TimeGame::loop() {
    if(show_instr_long_press()) {
        setFinished(true);
        return false;
    }

    unsigned long time = millis();
    unsigned long timeout_time = time + option_idle_time;

    while((time = millis()) < timeout_time){
        // Ask if ready to play (allows Back option)
        if(!prompt_ready()) {
            break; // Player chose Back, exit the game
        }

        // Reset for each game session
        round = 0;
        mean_time = 0;

        // Show round count
        title_prompt_int(FSTR("%3d Rounds"), TIME_GAME_ROUNDS, false, ROUNDS_SHOW_TIME);

        unsigned long best_time = renderBestTimePerMode(mode);
        if(best_time != DEFAULT_TIME) {
            title_prompt_string(FSTR("Beat %s"), copy_buffer, false, BEAT_SHOW_TIME);
        }

        // Play all TIME_GAME_ROUNDS rounds
        for(round = 1; round <= TIME_GAME_ROUNDS; round++) {
            display.clear();

            // Play one round
            delay(ROUND_DELAY);
            bool fault_protect = true;
            while(fault_protect){
                button_leds.activate_all(true);

                switch(mode){
                    case MODE_FLASH:
                        FSTR("Wait 4 FLASH");
                        break;
                    case MODE_SOUND:
                        FSTR("Wait 4 BEEP");
                        break;
                    case MODE_VIBRATION:
                        FSTR("Wait 4 BUZZ");
                        break;
                }
                display.scroll_string(fstring_buffer, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
                display.clear();
                button_leds.activate_all(false);

                int del = random(MIN_DELAY, MAX_DELAY+1);
                unsigned long delay_timeout = millis() + del;

                bool fault = false;
                unsigned long time_inner;
                while((time_inner = millis()) < delay_timeout)
                    if(digitalRead(ANY_BUTTON) == HIGH){
                        fault = true;
                        break;
                    }
                if(fault)
                    continue;
                fault_protect = false;
                
                // per-mode set up before starting timer
                switch(mode){
                    case MODE_FLASH:
                        panel_leds.begin_flash(false, 0);
                        break;
                }

                unsigned long start_time = micros();

                // per-mode actions to initiate stimulus
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
                    // per-mode actions to continue stimulus
                    switch(mode){
                        case MODE_FLASH:
                            panel_leds.step_flash(millis());
                            break;
                    }
                }

                unsigned long reaction_time = micros() - start_time;

                while(digitalRead(ANY_BUTTON) == HIGH){
                    // per-mode actions to complete stimulus while button still pressed
                    switch(mode){
                        case MODE_FLASH:
                            panel_leds.step_flash(millis());
                            break;
                    }
                }
                mean_time += reaction_time;

                micros_to_ms(copy_buffer, reaction_time);
                title_prompt_string(FSTR(" %s"), copy_buffer, false, ROUND_DELAY);
                delay(ROUND_DELAY);
                display.clear();
            }
        }

        // All rounds complete, show results
        while(button_pressed());

        mean_time /= TIME_GAME_ROUNDS;
        micros_to_ms(copy_buffer, mean_time);
        title_prompt_string(FSTR("SCORE %s"), copy_buffer, false, ROUND_DELAY);

        // save the best result per mode
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

        best_time = renderBestTimePerMode(mode);
        if(mean_time < best_time){
            micros_to_ms(copy_buffer, mean_time);
            title_prompt_string(FSTR("*NEW OVERALL BEST %s"), copy_buffer, true, ROUND_DELAY);

            display_win(TIME_WIN);

            add_to_purse(house_payout(TIME_WIN));
            display_purse();

            save_data();
        }

        // Reset timeout for next game
        timeout_time = time + option_idle_time;
    }
    
    return false; // Game finished due to timeout
}
