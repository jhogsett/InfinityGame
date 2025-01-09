#include <Arduino.h>
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"

void display_purse(){
	sprintf(display_buffer, FSTR("CASH $%s"), format_long(get_purse()));

	// long purse = get_purse();
	// if(purse > 999999) // # optimize strings
	// else
	// 	sprintf(display_buffer, FSTR("CASH $%ld"), purse);
	title_prompt(display_buffer, CASH_SHOW_TIMES, false, CASH_SHOW_DELAY);
}

void display_win(unsigned long win){
	sprintf(display_buffer, FSTR("*WIN $ %s"), format_long(win));

	// if(win > 9999999L) // # optimize strings
	// 	sprintf(display_buffer, FSTR("WIN$%ld"), win);
	// else if(win > 999999L)
	// 	sprintf(display_buffer, FSTR("WIN $%ld"), win);
	// else
	// 	sprintf(display_buffer, FSTR("*WIN $%ld"), win);
	title_prompt(display_buffer, WIN_SHOW_TIMES, true, ROUND_DELAY);
}

void display_jackpot(unsigned long win){
	sprintf(display_buffer, FSTR("** JACKPOT $%s"), format_long(win));
	title_prompt(display_buffer, JACKPOT_SHOW_TIMES, true, ROUND_DELAY);
}

char *format_long(long num){
	int negate = (num < 0L) ? -1 : 1;
	if(negate < 0)
		num *= -1L;

	int units = num % 1000L;
	int thous = ((num % 1000000L) - units) / 1000L;
	int mills = ((num % 1000000000L) - thous) / 1000000L;
	int bills =  ((num % 1000000000000L) - mills) / 1000000000L;

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

char *numeric_bet_str(long bet){
	sprintf(copy_buffer, FSTR("%ld"), bet);
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
		return numeric_bet_str(bet_amounts[bet]);
}
