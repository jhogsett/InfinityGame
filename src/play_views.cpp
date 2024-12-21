#include <Arduino.h>
#include "betting.h"
#include "buffers.h"
#include "play_data.h"
#include "play_views.h"
#include "prompts.h"
#include "utils.h"

void display_purse(){
	sprintf(display_buffer, "CASH $%ld", purse);
	title_prompt(display_buffer);
	delay(ROUND_DELAY);
}

void display_win(long win){
	sprintf(display_buffer, "WIN * $%ld", win);
	title_prompt(display_buffer, WIN_SHOW_TIMES, true);
	delay(ROUND_DELAY);
}

void display_jackpot(long win){
	sprintf(display_buffer, "JACKPOT *** $%ld", win);
	title_prompt(display_buffer, JACKPOT_SHOW_TIMES, true);
	delay(ROUND_DELAY);
}

char *numeric_bet_str(long bet){
	sprintf(copy_buffer, load_f_string(F("$%ld")), bet);
	return copy_buffer;
}

char *standard_bet_str(byte bet){
	if(bet == BET_ALL){
		sprintf(copy_buffer, load_f_string(F("ALL")));
		return copy_buffer;
	} else
		return numeric_bet_str(bet_amounts[bet]);
}
