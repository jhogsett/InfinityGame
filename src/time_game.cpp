#include <Arduino.h>
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
#include "utils.h"
#include "time_game.h"
#include "timeouts.h"
#include "debug.h"

unsigned long render_best_time_per_mode(char * buffer, int mode){
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

bool time_game(){
	if(title_prompt(FSTR("The TimeGame"), TITLE_SHOW_TIMES, true))
        return false;

    int mode = MODE_FLASH;
	const char *labels[] = {"LEDS", "BEEP", "BUZZ"};
	mode = toggle_prompt(FSTR("Choose  %s"), labels, mode, 3, 3);
    if(mode == -1)
        return false;

    unsigned long best_time = render_best_time_per_mode(copy_buffer, mode);
    if(best_time != DEFAULT_TIME)
        title_prompt_string(FSTR("Best Score %s"), copy_buffer, false, ROUND_DELAY);

	sprintf(display_buffer, FSTR("%3d Rounds"), TIMEGAME_ROUNDS);
	title_prompt(display_buffer, 1, false, ROUND_DELAY);

    unsigned long time = millis();
    unsigned long timeout_time = time + option_idle_time;

    bool instructed = false;
    while((time = millis()) < timeout_time){
        if(!prompt_ready())
        return false;

        if(!instructed){
            title_prompt_int(FSTR("%3d Rounds"), TIME_GAME_ROUNDS, false, ROUNDS_SHOW_TIME);
            instructed = true;
        }

        best_time = render_best_time_per_mode(copy_buffer, mode);
        if(best_time != DEFAULT_TIME)
            title_prompt_string(FSTR("Beat %s"), copy_buffer, false, BEAT_SHOW_TIME);

        display.clear();

        unsigned long mean = 0;
        for(byte i = 0; i < TIME_GAME_ROUNDS; i++){
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
                while((time = millis()) < delay_timeout)
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
                    // case MODE_SOUND:
                    //     break;
                    // case MODE_VIBRATION:
                    //     break;
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
                        // case MODE_SOUND:
                        //     break;
                        // case MODE_VIBRATION:
                        //     break;
                    }
                }

                unsigned long reaction_time = micros() - start_time;

                while(digitalRead(ANY_BUTTON) == HIGH){
                    // per-mode actions to complete stimulus while button still pressed
                    switch(mode){
                        case MODE_FLASH:
                            panel_leds.step_flash(millis());
                            break;
                        // case MODE_SOUND:
                        //     break;
                        // case MODE_VIBRATION:
                        //     break;
                    }
                }
                mean += reaction_time;

                micros_to_ms(copy_buffer, reaction_time);
                title_prompt_string(FSTR(" %s"), copy_buffer, false, ROUND_DELAY);
                delay(ROUND_DELAY);
                display.clear();
            }
        }

        while(button_pressed());

        mean /= TIME_GAME_ROUNDS;
        micros_to_ms(copy_buffer, mean);
        title_prompt_string(FSTR("SCORE %s"), copy_buffer, false, ROUND_DELAY);

        // save the best result per mode
        bool new_best_per_mode = false;
        char label[5];
        switch(mode){
            case MODE_FLASH:
                if(mean < best_time1){
                    best_time1 = mean;
                    new_best_per_mode = true;
                    load_f_string(F("LEDS"), label);
                }
                break;
            case MODE_SOUND:
                if(mean < best_time2){
                    best_time2 = mean;
                    new_best_per_mode = true;
                    load_f_string(F("BEEP"), label);
                }
                break;
            case MODE_VIBRATION:
                if(mean < best_time3){
                    best_time3 = mean;
                    new_best_per_mode = true;
                    load_f_string(F("BUZZ"), label);
                }
                break;
        }
        if(new_best_per_mode){
            micros_to_ms(copy_buffer, mean);
            title_prompt_string2(FSTR("*NEW BEST %s %s"), label, copy_buffer, true, ROUND_DELAY);
        }

        if(mean < best_time){
            best_time = mean;

            micros_to_ms(copy_buffer, mean);
            title_prompt_string(FSTR("*NEW OVERALL BEST %s"), copy_buffer, true, ROUND_DELAY);

            display_win(TIME_WIN);

            add_to_purse(house_payout(TIME_WIN));
            display_purse();

            save_data();
        }
    }

	return false;
}
