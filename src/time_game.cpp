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

bool time_game(){
	title_prompt(FSTR("The TimeGame"), TITLE_SHOW_TIMES, true);

    int mode = MODE_FLASH;
	const char *labels[] = {"LEDS", "BEEP", "BUZZ"};
	mode = toggle_prompt(FSTR("Stimul.  %s"), labels, mode, 3, 3);
    if(mode == -1)
        return false;

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
	sprintf(display_buffer, FSTR("Best Score %s ms"), copy_buffer);
	title_prompt(display_buffer, 1, false, ROUND_DELAY);

    unsigned long time = millis();
    unsigned long timeout_time = time + option_idle_time;

    while((time = millis()) < timeout_time){
        int response;
        const bool buttons[] = {false, true, false, true};
        response = button_led_prompt(FSTR("READY   Back"), buttons);
        if(response == 0 || response == -1 || response == RED_ID)
            return false;

        display.clear();
        delay(ROUND_DELAY);

        unsigned long mean = 0;
        for(byte i = 0; i < ROUNDS; i++){
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
                sprintf(display_buffer, FSTR("%s ms"), copy_buffer);
                display.scroll_string(display_buffer, DISPLAY_SHOW_TIME, DISPLAY_SCROLL_TIME);
                delay(ROUND_DELAY);
                display.clear();
            }
        }

        while(button_pressed());

        mean /= ROUNDS;
        micros_to_ms(copy_buffer, mean);
        sprintf(display_buffer, FSTR("SCORE %s ms"), copy_buffer);
        title_prompt(display_buffer, 1, false, ROUND_DELAY);

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
            sprintf(display_buffer, FSTR("*NEW BEST %s %s ms"), label, copy_buffer);
            title_prompt(display_buffer, 1, true, ROUND_DELAY);
        }

        if(mean < best_time){
            best_time = mean;

            micros_to_ms(copy_buffer, mean);
            sprintf(display_buffer, FSTR("*NEW OVERALL BEST %s ms"), copy_buffer);

            title_prompt(display_buffer, 1, true, ROUND_DELAY);

            display_win(TIME_WIN);

            add_to_purse(house_payout(TIME_WIN));
            display_purse();

            save_data();
        } else {
            micros_to_ms(copy_buffer, best_time);
            sprintf(display_buffer, FSTR("Overall Best %s ms"), copy_buffer);
            title_prompt(display_buffer, 1, false, ROUND_DELAY);
        }
    }

	return false;
}
