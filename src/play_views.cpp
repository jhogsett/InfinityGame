#include <Arduino.h>
#include "betting.h"
#include "buffers.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"

void display_purse(){
	if(purse > 999999)
		sprintf(display_buffer, FSTR("CASH$%ld"), purse);
	else
		sprintf(display_buffer, FSTR("CASH $%ld"), purse);
	title_prompt(display_buffer, CASH_SHOW_TIMES, false, ROUND_DELAY);
}

// void display_bank(){
// 	sprintf(display_buffer, FSTR("BANK $%ld"), bank);
// 	title_prompt(display_buffer, CASH_SHOW_TIMES, false, ROUND_DELAY);
// }

// void display_house(){
// 	sprintf(display_buffer, FSTR("HOUSE $%ld"), house);
// 	title_prompt(display_buffer, CASH_SHOW_TIMES, false, ROUND_DELAY);
// }

void display_win(long win){
	if(win > 999999L)
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
	sprintf(copy_buffer, FSTR("$%ld"), bet);
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
