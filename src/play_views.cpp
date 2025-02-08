#include <Arduino.h>
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"

bool display_scores(){
    unsigned long score;
    char label[5];
	char time_display[15];
    for(int i = 0; i < 3; i++){
        switch(i){
            case 0:
                score = best_time1;
                load_f_string(F("LEDS"), label);
                break;
            case 1:
                score = best_time2;
                load_f_string(F("BEEP"), label);
                break;
            case 2:
                score = best_time3;
                load_f_string(F("BUZZ"), label);
                break;
        }
		micros_to_ms(time_display, score);
        // sprintf(display_buffer, FSTR("%s SCORE %s ms"), label, time_display);
        // title_prompt(display_buffer, BALANCES_SHOW_TIMES, false, BALANCES_SHOW_DELAY);
        title_prompt_string2(FSTR("%s SCORE %s ms"), label, time_display, false, BALANCES_SHOW_DELAY);
    }
    return false;
}

bool display_balances(){
    long balance;
    char label[6];
    for(int i = 0; i < 6; i++){
        switch(i){
            case 0:
                balance = get_purse();
                load_f_string(F("CASH"), label);
                break;
            case 1:
                balance = get_vig();
                load_f_string(F("LOAN"), label);
                break;
            case 2:
                balance = get_house();
                load_f_string(F("HOUSE"), label);
                break;
            case 3:
                balance = get_gang();
                load_f_string(F("GANG"), label);
                break;
            case 4:
                balance = get_bank();
                load_f_string(F("BANK"), label);
                break;
            case 5:
                balance = get_sum();
                load_f_string(F("SUM"), label);
                break;
        }
        // sprintf(display_buffer, FSTR("%s $%s"), label, format_long(balance));
        // title_prompt(display_buffer, BALANCES_SHOW_TIMES, false, BALANCES_SHOW_DELAY);
        title_prompt_string2(FSTR("%s $%s"), label, format_long(balance), false, BALANCES_SHOW_DELAY);
    }
    return false;
}

void display_purse(int delay){
	// sprintf(display_buffer, FSTR("CASH $%s"), format_long(get_purse()));
	// title_prompt(display_buffer, CASH_SHOW_TIMES, false, delay || CASH_SHOW_DELAY);

    title_prompt_string(FSTR("CASH $%s"), format_long(get_purse()), false, delay || CASH_SHOW_DELAY);
}

void display_win(unsigned long win, int delay){
	// sprintf(display_buffer, FSTR("*WIN $%s"), format_long(win));
	// title_prompt(display_buffer, WIN_SHOW_TIMES, true, delay || WIN_SHOW_DELAY);

    title_prompt_string(FSTR("*WIN $%s"), format_long(win), true, delay || WIN_SHOW_DELAY);
}

void display_jackpot(unsigned long win){
	// sprintf(display_buffer, FSTR("** JACKPOT $%s"), format_long(win));
	// title_prompt(display_buffer, JACKPOT_SHOW_TIMES, true, ROUND_DELAY);

    title_prompt_string(FSTR("** JACKPOT $%s"), format_long(win), true, ROUND_DELAY);
}

char *standard_bet_str(byte bet){
	if(bet == BET_ALL){
		sprintf(copy_buffer, FSTR("ALL"));
		return copy_buffer;
	} else if(bet == BET_REPEAT){
		sprintf(copy_buffer, FSTR("RPT"));
		return copy_buffer;
	} else
		return format_long(bet_amounts[bet]);
}

// returns true if long-pressed
bool show_instr_long_press(){
    return title_prompt(FSTR("LONG PRESS EXITS"), 1, false, ROUND_DELAY);
}

// returns 0 for nice, 1 for rude, -1 for timeout or long press
int prompt_nice_or_rude(){
    bool rude;
    const bool buttons[] = {false, true, false, true};
    switch(button_led_prompt(FSTR("NICE or RUDE"), buttons)){
    case -1:
        // timeout
    case 0:
        // long press
        return -1;
    case 1:
        rude = false;
        break;
    case 2:
        rude = random(2) == 0 ? true : false;
        break;
    case 3:
        rude = true;
        break;
    }
    return rude ? 1 : 0;
}

// returns -1 on timeout or long press
int prompt_wpm(int wpm){
    const char *labels_wpm[] = {"13", "20", "35", "5"};
    int option;
    switch(wpm){
        case 13:
            option = 0;
            break;
        case 20:
            option = 1;
            break;
        case 35:
            option = 2;
            break;
        case 5:
            option = 3;
            break;
    }

    int result = toggle_prompt(FSTR("WPM     %s"), labels_wpm, option, 3, 4);
    if(result == -1)
        return -1;

    switch(result){
        case 0:
            wpm = 13;
            break;
        case 1:
            wpm = 20;
            break;
        case 2:
            wpm = 35;
            break;
        case 3:
            wpm = 5;
            break;
    }

    return wpm;
}