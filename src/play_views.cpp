#include <Arduino.h>
#include "bank.h"
#include "betting.h"
#include "buffers.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"

void display_purse(){
	if(get_purse() > 999999) // # optimize strings
		sprintf(display_buffer, FSTR("CASH$%ld"), get_purse());
	else
		sprintf(display_buffer, FSTR("CASH $%ld"), get_purse());
	title_prompt(display_buffer, CASH_SHOW_TIMES, false, CASH_SHOW_DELAY);
}

void display_win(long win){
	if(win > 999999L) // # optimize strings
		sprintf(display_buffer, FSTR("WIN$%ld"), win);
	else if(win > 99999L)
		sprintf(display_buffer, FSTR("WIN $%ld"), win);
	else
		sprintf(display_buffer, FSTR("WIN* $%ld"), win);
	title_prompt(display_buffer, WIN_SHOW_TIMES, true, ROUND_DELAY);
}

void display_jackpot(long win){
	sprintf(display_buffer, FSTR("JACKPOT *** $%ld"), win);
	title_prompt(display_buffer, JACKPOT_SHOW_TIMES, true, ROUND_DELAY);
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
