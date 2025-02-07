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
        sprintf(display_buffer, FSTR("%s SCORE %s ms"), label, time_display);
        title_prompt(display_buffer, BALANCES_SHOW_TIMES, false, BALANCES_SHOW_DELAY);
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
        sprintf(display_buffer, FSTR("%s $%s"), label, format_long(balance));
        title_prompt(display_buffer, BALANCES_SHOW_TIMES, false, BALANCES_SHOW_DELAY);
    }
    return false;
}

void display_purse(int delay){
	sprintf(display_buffer, FSTR("CASH $%s"), format_long(get_purse()));
	title_prompt(display_buffer, CASH_SHOW_TIMES, false, delay || CASH_SHOW_DELAY);
}

void display_win(unsigned long win, int delay){
	sprintf(display_buffer, FSTR("*WIN $%s"), format_long(win));
	title_prompt(display_buffer, WIN_SHOW_TIMES, true, delay || WIN_SHOW_DELAY);
}

void display_jackpot(unsigned long win){
	sprintf(display_buffer, FSTR("** JACKPOT $%s"), format_long(win));
	title_prompt(display_buffer, JACKPOT_SHOW_TIMES, true, ROUND_DELAY);
}

char *format_long(long num, long basis){
	int negate = (num < 0L) ? -1 : 1;
	if(negate < 0)
		num *= -1L;

	if(basis == 0)
		basis = MONEY_BASIS;

	// this captures and removes the lower three digits in the expanded basis of the number
	// plus removes the basis factor and 1000 for the remaining splits
	long basis_factor = 1000L / basis;
	int units = num % (basis_factor);
	num = (num - units) / (basis_factor);
	units *= basis;

	int thous = ((num % 1000L) - 0) / 1L;
	int mills = ((num % 1000000L) - thous) / 1000L;
	int bills = ((num % 1000000000L) - mills) / 1000000L;

	if(bills > 0)
		sprintf(copy_buffer, FSTR("%d,%03d,%03d,%03d"), bills*negate, mills, thous, units);
	else if(mills > 0)
		sprintf(copy_buffer, FSTR("%d,%03d,%03d"), mills*negate, thous, units);
	else if(thous > 0)
		sprintf(copy_buffer, FSTR("%d,%03d"), thous*negate, units);
	else
		sprintf(copy_buffer, FSTR("%d"), units*negate);

	return copy_buffer;
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
    sprintf(display_buffer, FSTR("LONG PRESS EXITS"));
    return title_prompt(display_buffer, 1, false, ROUND_DELAY);
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